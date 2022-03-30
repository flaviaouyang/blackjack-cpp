#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace std;

enum Rank
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

enum Type
{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

int computerWon = 0;
int humanWon = 0;

int randFunc(int i)
{
    return rand() % i;
}

class Card
{
public:
    // constructor
    Card(Rank rank, Type type) : rank(rank), type(type) {}

    // methods

    // return the numeric value of the card
    int getValue() const
    {
        const int value[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
        return value[rank];
    }

    // display card's rank and type
    void displayCard() const
    {
        const string typeNames[] = {"C", "D", "H", "S"};
        const string rankNames[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
        cout << rankNames[rank] << typeNames[type] << " ";
    }

private:
    Rank rank;
    Type type;
};

class Hand
{
public:
    // add a card to the hand
    void add(Card card)
    {
        aHand.push_back(card);
    }

    // remove all cards
    void clear()
    {
        aHand.clear();
    }

    // get total numeric value of a Hand
    int getTotal() const
    {
        int sum = 0;
        for (int i = 0; i < aHand.size(); i++)
        {
            // check whether 1 or 11 is more preferrable
            if (aHand.at(i).getValue() == 11)
            {
                if (sum > 10)
                {
                    sum = sum + 1;
                }
                else
                {
                    sum = sum + aHand.at(i).getValue();
                }
            }
            else
            {
                sum = sum + aHand.at(i).getValue();
            }
        }
        return sum;
    }

    vector<Card> getHand() const
    {
        return aHand;
    }

private:
    vector<Card> aHand;
};

class Deck
{
public:
    vector<Card> getDeck() const
    {
        return aDeck;
    }

    // generate 52 cards
    void populate()
    {
        aDeck.clear();
        for (int i = CLUBS; i <= SPADES; i++)
        {
            for (int j = ACE; j <= KING; j++)
            {
                Type tp = static_cast<Type>(i);
                Rank rk = static_cast<Rank>(j);
                aDeck.push_back(Card(rk, tp));
            }
        }
    }

    // shuffle the cards in the deck
    void shuffle()
    {
        srand(unsigned(time(NULL)));
        random_shuffle(aDeck.begin(), aDeck.end(), randFunc);
    }

    // deal a card to the hand
    Hand deal(Hand hand)
    {
        hand.add(aDeck.front());
        aDeck.erase(aDeck.begin());
        return hand;
    }

private:
    vector<Card> aDeck;
    void clear() { aDeck.clear(); }
};

class AbstractPlayer
{
public:
    virtual bool isDrawing() const = 0;
    bool isBusted(int sum)
    {
        return sum > 21;
    }
};

class HumanPlayer : public AbstractPlayer
{
public:
    bool isDrawing() const
    {
        char answer;
        while (answer != 'y' && answer != 'n')
        {
            cout << "Do you want to draw? (y/n): ";
            cin >> answer;
        }
        return answer == 'y';
    }

    void announce(int result)
    {
        switch (result)
        {
        case 1:
            cout << "Casino wins." << endl;
            break;
        case 2:
            cout << "Player wins." << endl;
            break;
        case 3:
            cout << "Push: No one wins." << endl;
            break;
        }
    }

    void setHand(Hand playerHand)
    {
        this->playerHand = playerHand;
    }

private:
    Hand playerHand;
};

class ComputerPlayer : public AbstractPlayer
{
public:
    bool isDrawing() const
    {
        int total = computerWon + humanWon;
        double win = (double)computerWon / total;
        // check for winning rate
        if (win >= 0.55)
        {
            return score <= 16;
        }
        else
        {
            // trick if winning rate is less than 55%
            int next = trick.getDeck().front().getValue();
            if (next != 11)
            {
                if (score + next <= 21)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (score + 1 <= 21)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    void setScore(int score)
    {
        this->score = score;
    }
    void setDeck(Deck trick)
    {
        this->trick = trick;
    }

private:
    Hand computerHand;
    Deck trick;
    int score;
};

class BlackJackGame
{
public:
    void play()
    {
        // count
        int playerCnt;
        int casinoCnt;

        // get 52 cards and shuffle
        m_deck.populate();
        m_deck.shuffle();

        // two hand
        Hand playerHand;
        Hand computerHand;

        // deal open cards
        playerHand = m_deck.deal(playerHand);
        computerHand = m_deck.deal(computerHand);
        playerHand = m_deck.deal(playerHand);

        // display casino cards
        cout << "Casino: ";
        computerHand.getHand().front().displayCard();
        cout << "[" << computerHand.getTotal() << "]" << endl;

        // display player cards
        cout << "Player: ";
        for (int i = 0; i < playerHand.getHand().size(); i++)
        {
            playerHand.getHand().at(i).displayCard();
        }
        cout << "[" << playerHand.getTotal() << "]" << endl;

        m_player.setHand(playerHand);

        // prompt human to draw cards
        bool human = m_player.isDrawing();
        while (human)
        {
            playerHand = m_deck.deal(playerHand);

            cout << "Player: ";
            for (int i = 0; i < playerHand.getHand().size(); i++)
            {
                playerHand.getHand().at(i).displayCard();
            }
            cout << "[" << playerHand.getTotal() << "]" << endl;

            // check if player busted
            if (m_player.isBusted(playerHand.getTotal()))
            {
                cout << "Player busts" << endl;
                m_player.announce(1);
                computerWon = computerWon + 1;
                int total = computerWon + humanWon;
                double win = (double)humanWon / total;

                cout << "Your winning rate is " << win * 100 << "%" << endl;
                cout << endl
                     << endl;
                return;
            }
            human = m_player.isDrawing();
        }

        playerCnt = playerHand.getTotal();
        casinoCnt = computerHand.getTotal();
        m_casino.setScore(casinoCnt);
        m_casino.setDeck(m_deck);
        while (m_casino.isDrawing())
        {
            computerHand = m_deck.deal(computerHand);
            cout << "Casino: ";
            for (int i = 0; i < computerHand.getHand().size(); i++)
            {
                computerHand.getHand().at(i).displayCard();
            }
            cout << "[" << computerHand.getTotal() << "]" << endl;

            // check if computer busted
            if (m_casino.isBusted(computerHand.getTotal()))
            {
                cout << "Casino busts" << endl;
                m_player.announce(2);
                humanWon = humanWon + 1;
                int total = computerWon + humanWon;
                double win = (double)humanWon / total;

                cout << "Your winning rate is " << win * 100 << "%" << endl;
                cout << endl
                     << endl;
                return;
            }
            casinoCnt = computerHand.getTotal();
            m_casino.setScore(casinoCnt);
            m_casino.setDeck(m_deck);
        }

        if (casinoCnt > playerCnt)
        {
            computerWon = computerWon + 1;
            m_player.announce(1);
        }
        else if (casinoCnt < playerCnt)
        {
            humanWon = humanWon + 1;
            m_player.announce(2);
        }
        else
        {
            m_player.announce(3);
        }

        int total = computerWon + humanWon;
        double win = (double)humanWon / total;

        cout << "Your winning rate is " << win * 100 << "%" << endl;

        cout << endl
             << endl;
    }

private:
    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer m_player;
};

int main()
{
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl
         << endl;

    BlackJackGame game;

    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';

    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl
             << endl;
        playAgain = (answer == 'y' ? true : false);
    }

    cout << "Game over!";
    return 0;
}