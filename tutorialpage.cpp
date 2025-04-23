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
            "We will run you through a quick tutorial to get you ready for the game."
        },
        {
            ":/tutorial/tutorial-mtg/p2.png",
            "There are many different card types in MTG: Creature, Land, Instant, etc.\n"
            "You can figure out what any card is by reading the type line. The first line of the card has the card’s name in the top left, "
            "and its mana cost in the top right. The middle line has the card’s type and expansion symbol. The first word in the type line "
            "tells you what kind of card it is. A creature will say “Creature” followed by the subtype of creature it is.\n\n"
            "The bottom half of a card consists of the card’s text box, where you’ll find any ability or effect the card has. "
            "If the card is a creature, it will have its power and toughness in the bottom right. Walking Corpse has two power and two toughness. "
            "Power and toughness are often shortened to just the numbers when playing, so Walking Corpse would be called a 2/2."
        },
        {
            ":/tutorial/tutorial-mtg/p3.png",
            "On each of your turns, you can play one land. You can use a land to make one mana of a corresponding color by tapping it once each turn.\n\n"
            "Plains make white mana, Islands make blue mana, Swamps make black mana, Mountains make red mana, and Forests make green mana."
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
            "Next, you would love to experience the game yourself!\n\n"
            "You can learn more about other elements of the game while playing!\n\n"
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
