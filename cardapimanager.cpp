#include "cardapimanager.h"
#include "card.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QUrlQuery>
#include <QImageReader>
#include <QBuffer>

CardAPIManager::CardAPIManager(QObject *parent)
    : QObject(parent),
    manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished,
            this, &CardAPIManager::onFinished);
}

void CardAPIManager::fetchCards()
{
    QUrl url("https://api.scryfall.com/cards");
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);
    pendingReplies[reply] = ReplyType::CardsList;
}

void CardAPIManager::fetchCardByName(const QString &cardName)
{
    QUrl url("https://api.scryfall.com/cards/named");
    QUrlQuery query;
    query.addQueryItem("exact", cardName);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    pendingReplies[reply] = ReplyType::SingleCard;
}

void CardAPIManager::fetchCardsByNames(const QStringList &cardNames)
{
    batchResults.clear();
    remainingBatchCards = cardNames.size();

    emit batchFetchStarted(cardNames.size());

    for (const QString &cardName : cardNames) {
        fetchCardByName(cardName);
    }
}

void CardAPIManager::onFinished(QNetworkReply *reply)
{
    ReplyType type = pendingReplies.value(reply, ReplyType::Unknown);
    pendingReplies.remove(reply);

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());

        if (type == ReplyType::SingleCard && remainingBatchCards > 0) {
            remainingBatchCards--;

            if (remainingBatchCards == 0) {
                emit batchCardsFetched(batchResults);
            }
        }

        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (jsonDoc.isNull()) {
        emit errorOccurred("Invalid JSON response.");
        reply->deleteLater();
        return;
    }

    if (type == ReplyType::CardsList) {
        if (!jsonDoc.isObject()) {
            emit errorOccurred("Invalid JSON response format for cards list.");
            reply->deleteLater();
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray cardsArray = jsonObj["data"].toArray();
        emit cardsFetched(cardsArray);
    }
    else if (type == ReplyType::SingleCard) {
        if (!jsonDoc.isObject()) {
            emit errorOccurred("Invalid JSON response format for single card.");
            reply->deleteLater();
            return;
        }

        QJsonObject cardObject = jsonDoc.object();
        Card card = parseCardFromJson(cardObject);

        if (cardObject.contains("image_uris")) {
            QString imageUrl = cardObject["image_uris"].toObject()["normal"].toString();
            downloadCardImage(card, imageUrl);
        } else {
            emit cardFetched(card);

            if (remainingBatchCards > 0) {
                batchResults.append(card);
                remainingBatchCards--;

                emit batchFetchProgress(batchResults.size());

                if (remainingBatchCards == 0) {
                    emit batchCardsFetched(batchResults);
                }
            }
        }
    }
    else if (type == ReplyType::CardImage) {
        Card card = imageDownloads.value(reply);
        imageDownloads.remove(reply);

        QImage image;
        if (image.loadFromData(responseData)) {
            card.image = image;
        }

        emit cardFetched(card);

        if (remainingBatchCards > 0) {
            batchResults.append(card);
            remainingBatchCards--;

            emit batchFetchProgress(batchResults.size());

            if (remainingBatchCards == 0) {
                emit batchCardsFetched(batchResults);
            }
        }
    }

    reply->deleteLater();
}

void CardAPIManager::downloadCardImage(const Card &card, const QString &imageUrl)
{
    QUrl url(imageUrl);
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);
    pendingReplies[reply] = ReplyType::CardImage;
    imageDownloads[reply] = card;
}

Card CardAPIManager::parseCardFromJson(const QJsonObject &cardJson)
{
    Card card;

    card.name = cardJson["name"].toString();
    card.description = cardJson["oracle_text"].toString();

    if (cardJson.contains("mana_cost")) {
        QString manaCost = cardJson["mana_cost"].toString();
        card.cost = parseManaSymbols(manaCost);
    }

    if (cardJson.contains("colors")) {
        card.color = determineCardColor(cardJson["colors"].toArray());
    } else {
        card.color = ManaType::COLORLESS;
    }

    if (cardJson.contains("type_line")) {
        QString typeText = cardJson["type_line"].toString();
        card.type = determineCardType(typeText);

        card.isPermanent = (card.type == CardType::CREATURE ||
                            card.type == CardType::ARTIFACT ||
                            card.type == CardType::ENCHANTMENT ||
                            card.type == CardType::PLANESWALKER ||
                            card.type == CardType::LAND);
    }

    if (card.type == CardType::CREATURE) {
        if (cardJson.contains("power")) {
            card.power = cardJson["power"].toString().toInt();
        }
        if (cardJson.contains("toughness")) {
            card.toughness = cardJson["toughness"].toString().toInt();
        }
    }

    if (cardJson.contains("keywords")) {
        QJsonArray keywordsArray = cardJson["keywords"].toArray();
        card.keywords = parseKeywords(keywordsArray);
    }

    card.isTapped = false;

    return card;
}

QMap<ManaType, int> CardAPIManager::parseManaSymbols(const QString &manaCost)
{
    QMap<ManaType, int> costMap;

    costMap[ManaType::WHITE] = 0;
    costMap[ManaType::BLUE] = 0;
    costMap[ManaType::BLACK] = 0;
    costMap[ManaType::RED] = 0;
    costMap[ManaType::GREEN] = 0;
    costMap[ManaType::COLORLESS] = 0;
    costMap[ManaType::ANY] = 0;

    for (int i = 0; i < manaCost.length(); i++) {
        if (manaCost[i] == '{') {
            i++;
            if (i < manaCost.length()) {
                if (manaCost[i] == 'W') costMap[ManaType::WHITE]++;
                else if (manaCost[i] == 'U') costMap[ManaType::BLUE]++;
                else if (manaCost[i] == 'B') costMap[ManaType::BLACK]++;
                else if (manaCost[i] == 'R') costMap[ManaType::RED]++;
                else if (manaCost[i] == 'G') costMap[ManaType::GREEN]++;
                else if (manaCost[i] == 'C') costMap[ManaType::COLORLESS]++;
                else if (manaCost[i].isDigit()) {
                    // For costs like {2}, add to the generic mana count
                    QString number;
                    while (i < manaCost.length() && manaCost[i].isDigit()) {
                        number += manaCost[i];
                        i++;
                    }
                    i--; // Adjust for the next iteration

                    bool ok;
                    int genericAmount = number.toInt(&ok);
                    if (ok) {
                        costMap[ManaType::ANY] += genericAmount;
                    }
                }
            }
        }
    }

    return costMap;
}

ManaType CardAPIManager::determineCardColor(const QJsonArray &colorsArray)
{
    if (colorsArray.isEmpty()) return ManaType::COLORLESS;

    if (colorsArray.size() == 1) {
        QString color = colorsArray[0].toString();
        if (color == "W") return ManaType::WHITE;
        if (color == "U") return ManaType::BLUE;
        if (color == "B") return ManaType::BLACK;
        if (color == "R") return ManaType::RED;
        if (color == "G") return ManaType::GREEN;
    }
}

CardType CardAPIManager::determineCardType(const QString &typeLine)
{
    if (typeLine.contains("Creature", Qt::CaseInsensitive)) return CardType::CREATURE;
    if (typeLine.contains("Instant", Qt::CaseInsensitive)) return CardType::INSTANT;
    if (typeLine.contains("Sorcery", Qt::CaseInsensitive)) return CardType::SORCERY;
    if (typeLine.contains("Artifact", Qt::CaseInsensitive)) return CardType::ARTIFACT;
    if (typeLine.contains("Enchantment", Qt::CaseInsensitive)) return CardType::ENCHANTMENT;
    if (typeLine.contains("Planeswalker", Qt::CaseInsensitive)) return CardType::PLANESWALKER;
    if (typeLine.contains("Land", Qt::CaseInsensitive)) return CardType::LAND;
}

QVector<PropertyType> CardAPIManager::parseKeywords(const QJsonArray &keywordsArray)
{
    QVector<PropertyType> keywords;

    for (const QJsonValue &value : keywordsArray) {
        QString keyword = value.toString();

        if (keyword == "Flying") keywords.append(PropertyType::FLYING);
        else if (keyword == "First strike") keywords.append(PropertyType::FIRST_STRIKE);
        else if (keyword == "Deathtouch") keywords.append(PropertyType::DEATHTOUCH);
        else if (keyword == "Trample") keywords.append(PropertyType::TRAMPLE);
        else if (keyword == "Lifelink") keywords.append(PropertyType::LIFELINK);
        else if (keyword == "Haste") keywords.append(PropertyType::HASTE);
    }
    return keywords;
}
