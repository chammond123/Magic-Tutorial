#include "cardapimanager.h"
#include "card.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QImageReader>
#include <QBuffer>

CardAPIManager::CardAPIManager(QObject *parent)
    : QObject(parent),
    manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished,
            this, &CardAPIManager::onFinished);

    symbolMap = {
                 {"{G}", "Green mana"},
                 {"{R}", "Red mana"},
                 {"{U}", "Blue mana"},
                 {"{B}", "Black mana"},
                 {"{W}", "White mana"},
                 {"{C}", "Colorless mana"},
                 {"{T}", "Tap"},
                 {"{Q}", "Untap"},
                 {"{X}", "X"},
                 {"{0}", "0"},
                 {"{1}", "1"},
                 {"{2}", "2"},
                 {"{3}", "3"},
                 {"{4}", "4"},
                 {"{5}", "5"},
                 {"{6}", "6"},
                 {"{7}", "7"},
                 {"{8}", "8"},
                 {"{9}", "9"},
                 {"{10}", "10"},
                 {"{11}", "11"},
                 {"{12}", "12"},
                 {"{13}", "13"},
                 {"{14}", "14"},
                 {"{15}", "15"},
                 {"{16}", "16"},
                 {"{17}", "17"},
                 {"{18}", "18"},
                 {"{19}", "19"},
                 {"{20}", "20"},
                 {"{∞}", "Infinity"},
                 };
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
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
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

void CardAPIManager::downloadCardImage(const Card &card, const QString &imageUrl)
{
    QUrl url(imageUrl);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    pendingReplies[reply] = ReplyType::CardImage;
    imageDownloads[reply] = card;
}

void CardAPIManager::onFinished(QNetworkReply *reply)
{
    ReplyType type = pendingReplies.value(reply, ReplyType::Unknown);
    pendingReplies.remove(reply);

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        if ((type == ReplyType::SingleCard || type == ReplyType::CardImage) && remainingBatchCards > 0) {
            remainingBatchCards--;
            if (remainingBatchCards == 0) {
                emit batchCardsFetched(batchResults);
            }
        }
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();


    if (type == ReplyType::CardsList) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isObject()) {
            emit errorOccurred("Invalid JSON response format for cards list.");
            reply->deleteLater();
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("data")) {
            emit errorOccurred("Missing 'data' field in cards list JSON.");
            reply->deleteLater();
            return;
        }
        QJsonArray cardsArray = jsonObj["data"].toArray();
        emit cardsFetched(cardsArray);
    }
    else if (type == ReplyType::SingleCard) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isObject()) {
            emit errorOccurred("Invalid JSON response format for single card.");
            reply->deleteLater();
            return;
        }
        QJsonObject cardObject = jsonDoc.object();
        Card card = parseCardFromJson(cardObject);

        if (cardObject.contains("image_uris") && cardObject["image_uris"].isObject()) {
            QJsonObject imageUris = cardObject["image_uris"].toObject();
            QString imageUrl = imageUris["normal"].toString();
            if (!imageUrl.isEmpty()) {
                downloadCardImage(card, imageUrl);
                reply->deleteLater();
                return;
            }
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
    else if (type == ReplyType::CardImage) {
        Card card = imageDownloads.value(reply);
        imageDownloads.remove(reply);

        QImage image;
        if (image.loadFromData(responseData)) {
            card.image = image;
        } else {
            emit errorOccurred("Failed to load card image from data.");
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

Card CardAPIManager::parseCardFromJson(const QJsonObject &cardJson)
{
    Card card;

    card.name = cardJson["name"].toString();

    QRegularExpression regex(R"(\{[^}]+\})");
    QString rawDescription = cardJson["oracle_text"].toString();
    QString processedDescription = rawDescription;

    // Replace all symbols like {X} with their mapped values
    QRegularExpressionMatchIterator matches = regex.globalMatch(rawDescription);
    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        QString symbol = match.captured(0);
        if (symbolMap.contains(symbol)) {
            processedDescription.replace(symbol, symbolMap[symbol]);
        }
    }

    card.description = processedDescription;

    if (cardJson.contains("mana_cost")) {
        QString manaCost = cardJson["mana_cost"].toString();
        card.cost = parseManaSymbols(manaCost);
    }

    if (cardJson.contains("color_identity")) {
        card.color = determineCardColor(cardJson["color_identity"].toArray());
    } else {
        card.color = ManaType::COLORLESS;
    }

    if (cardJson.contains("type_line")) {
        QString typeText = cardJson["type_line"].toString().toUtf8();
        card.type = determineCardType(typeText);

        card.isPermanent = (card.type == CardType::CREATURE ||
                            card.type == CardType::ARTIFACT ||
                            card.type == CardType::ENCHANTMENT ||
                            card.type == CardType::PLANESWALKER ||
                            card.type == CardType::LAND);

        card.isLand = (card.type == CardType::LAND);
    }

    if (card.type == CardType::CREATURE) {
        if (cardJson.contains("power")) {
            card.power = cardJson["power"].toString().toInt();
        }
        if (cardJson.contains("toughness")) {
            card.toughness = cardJson["toughness"].toString().toInt();
            card.currHealth = card.toughness;
        }
    }

    if (cardJson.contains("keywords")) {
        QJsonArray keywordsArray = cardJson["keywords"].toArray();
        card.keywords = parseKeywords(keywordsArray);
    }

    if (cardJson.contains("flavor_text")){
        QString flavorText = cardJson["flavor_text"].toString().toUtf8();
        card.flavorText = flavorText;
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
                    QString number;
                    while (i < manaCost.length() && manaCost[i].isDigit()) {
                        number += manaCost[i];
                        i++;
                    }
                    i--;
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
    return ManaType::COLORLESS;
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
    return CardType::SORCERY;
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

