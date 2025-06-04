#include "tutorialpage.h"
#include "ui_tutorialpage.h"
#include <QPushButton>

TutorialPage::TutorialPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TutorialPage)
{
    ui->setupUi(this);
    setWindowTitle("Tutorial");

    slides = {
        {
            ":/tutorial/tutorial-mtg/p1.jpg",
            "Magic is a collectible trading card game of fun-filled, strategic games to play with friends old and new. "
            "Welcoming worldbuilders, narrative lovers, and gameplay enthusiasts alike, Magic has something for everyone and countless ways to play. "
            "Whether you're sitting at the kitchen table, playing online, or battling in a high-stakes competition, "
            "there's a place for you in the world of Magic: The Gathering.\n\n"
            "We will run you through a quick tutorial to get you ready for the game. Just hit the right arrow to progress once you're done reading!"
        },
        {
            ":/tutorial/tutorial-mtg/phases_img.png",
            "During the game, there are a few phases, which will be gone into more in-depth when you play the game. "
            "There are, however, three important ideas to know about – Priority, the Active Player, and the Stack:\n\n"
            "• Priority and passing priority: A turn-based system that determines who may cast spells or activate abilities. "
            "Each player in turn gets priority and may act, then passes it to the other player. "
            "When both players pass priority without taking an action, the top object on the stack resolves.\n\n"
            "• The active player: The player whose turn it is. They can play lands, cast spells, and receive priority first in each step and phase. (Note, you can not play land if you aren't the active player.\n\n"
            "• The stack: A “waiting area” where spells and abilities go before they resolve, operating in last-in, first-out order. "
            "Whenever a player casts a spell or activates an ability, it’s placed on the stack and players get priority to respond before the top object resolves."
        },

        {
            ":/tutorial/tutorial-mtg/card_full_highlight.png",
            "There are many different cards in Magic: The Gathering. In fact, there are over 27,000! It's nearly impossible to know all of them,"
            " but you don't need to, you just need to know how to read one. The card displayed is one you will see during your practice game, so let's"
            " read what there is to know about this card."
        },
        {
            ":/tutorial/tutorial-mtg/card_name_highlight.png",
            "This is the name of the card. It is not crucial to any game factors, but is good to know, and you'll be familiar with many more names of cards as you play."
        },
        {
            ":/tutorial/tutorial-mtg/card_mana_highlight.png",
            "This is the mana cost of the card. Most cards in MTG have a cost (mana) you must pay before playing them. The mana cost is represented by symbols showing the types of mana required. If you see a number, "
            "it means that it needs that much of ANY type of mana. For instance, this card requires one red mana (the fire icon) and 3 of any of your available mana. We'll get into mana a bit later."
        },
        {
            ":/tutorial/tutorial-mtg/card_type_highlight.png",
            "This is the type of the card. There are many different card types in MTG: Creature, Land, Instant, etc. Some are permanent (will stay on the battlefield after you play them,"
            " and some are not. The first word in the type line "
            "tells you what kind of card it is. A creature will say “Creature” followed by the subtype of creature it is. Each have their own special qualities: \n\n"
             "• Creature – A permanent card you cast that can attack and block your opponent (and can also be attacked and blocked).\n"
             "• Land – A resource you play to generate mana by tapping it.\n"
             "• Sorcery – A one-time spell you can cast only during your main phase.\n"
             "• Instant – A one-time spell you can cast at almost any time, even on your opponent’s turn.\n"
             "• Artifact – A permanent card that stays on the battlefield and often has abilities.\n"
             "• Enchantment – A permanentcard  that creates a continuous effect on the battlefield.\n"
             "• Planeswalker – A powerful ally with loyalty counters that can activate abilities each turn. We won't be getting into this one, but look it up if you're curious!\n"
        },
        {
            ":/tutorial/tutorial-mtg/card_description_highlight.png",
            "This is the description of the card. This card doesn't have anything besides just some lore about the card, but many cards have abilites, and this is where they will be displayed."
            " There are too many abilities to know all of them, so make sure to read this section any time you encounter a new card to familiarize yourself with its ability."
        },
        {
            ":/tutorial/tutorial-mtg/card_stats_highlight.png",
            "Finally, some cards (generally creatures) have a power and toughness, displayed in that order (power/toughness). The power indicates how much damage it can do to another card,"
            " and toughness indicates how much damage that card can take in one turn without going to the graveyard (being eliminated). A few notes: \n\n"
            "• All damage in MTG is done in one turn, you can't chip away damage at a unit over multiple turns. To destroy a unit, you must deal more damage to it than it has toughness\n"
            "• When creatures are destroyed, they go to the graveyard. Some abilities can bring cards back from the graveyard, so it is not always a permanent defeat. \n"
            "• Some abilities will cause a creature to be exiled, which generally means it's not coming back! Of course, some abilities can, but you may encounter those occasionally. \n"
        },
        {
            ":/tutorial/tutorial-mtg/p3.png",
            "On each of your turns, you can play one land card from your hand onto the battlefield.\n\n"
            "Lands are your main source of mana. You tap a land (turn it sideways) to produce one mana of a specific color.\n"
            "• Plains make white mana\n"
            "• Islands make blue mana\n"
            "• Swamps make black mana\n"
            "• Mountains make red mana\n"
            "• Forests make green mana\n\n"
            "Most cards cost mana to play. Recall that the amount and color of mana required is shown in the top right of each card.\n"
            "To cast a card, you tap the necessary lands to generate the correct colors of mana, then spend that mana to play the card.\n"
            "Tapped lands can’t be used again until they untap at the start of your next turn.\n\n"
            "At the start of the game, you’ll mostly be casting cheaper cards—but as you play more lands, you’ll be able to summon stronger spells and creatures!"
        },
        {
            ":/tutorial/tutorial-mtg/p4.png",
            "To start playing, all you really need is a place to play, someone to play against, and a deck. Each player starts the game with 20 life. "
            "The goal is to reduce your opponent’s life total to zero before they do the same to you.\n\n"
            "Before starting, determine who goes first in your game, and make sure to shuffle your deck. Once you put your deck within easy reach, "
            "it’s now known as the library.\n\n"
            "Draw an opening hand of seven cards from the top of your library. If you’re the first player, you won’t draw an extra card on your first turn. "
            "But after that, each player draws a card at the start of their turn.\n\n"
            "In a game of Magic, you alternate taking turns with your opponent. Each turn consists of a handful of phases that you proceed through. "
            "There are five phases: the beginning phase, the first main phase, the combat phase, the second main phase, and the ending phase. "
            "You will cycle through all five in order each turn, whether you have actions to make in them or not."
        },
        {
            ":/tutorial/tutorial-mtg/p5.png",
            "Now that you've read everything, it's time to experience the game yourself!\n\n"
            "You can learn more about other elements of the game while playing!\n\n"
            "Feel free to read back until you're comfortable with this information (it's a lot, we know) \n\n"
            "Go back to the menu and click play to start diving into the MTG!"
        }
    };

    connect(ui->backButton, &QPushButton::clicked, this, &TutorialPage::showPreviousSlide);
    connect(ui->nextButton, &QPushButton::clicked, this, &TutorialPage::showNextSlide);
    connect(ui->backToMenuButton, &QPushButton::clicked, this, &TutorialPage::backToMenu);

    ui->descriptionBox->setReadOnly(true);
    updateSlide();
}

TutorialPage::~TutorialPage() {
    delete ui;
}

void TutorialPage::updateSlide() {
    const TutorialSlide& slide = slides[currentIndex];

    ui->imageLabel->setPixmap(QPixmap(slide.imagePath).scaled(
        ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->descriptionBox->setText(slide.text);

    ui->backToMenuButton->setEnabled(currentIndex > 0);
    ui->nextButton->setEnabled(currentIndex < slides.size() - 1);
}

void TutorialPage::showNextSlide() {
    if (currentIndex < slides.size() - 1) {
        ++currentIndex;
        updateSlide();
    }
}

void TutorialPage::showPreviousSlide() {
    if (currentIndex > 0) {
        --currentIndex;
        updateSlide();
    }
}
