#include <array>
#include <iostream>
#include <optional>
#include <random>
#include <string>

enum class Choice
{
    Rock,
    Paper,
    Scissors,
};

std::ostream& operator<<(std::ostream& os, const Choice& choice)
{
    using namespace std::string_literals;
    std::array const choice_str{"Rock"s, "Paper"s, "Scissors"s};
    os << choice_str[static_cast<int>(choice)];
    return os;
    
    // switch (choice)
    // {
    // case Choice::Rock:
    //     os << "Rock";
    //     break;
    // case Choice::Paper:
    //     os << "Paper";
    //     break;
    // case Choice::Scissors:
    //     os << "Scissors";
    //     break;
    // }
    // return os;
}

std::optional<Choice> human_turn()
{
    int choice;
    std::cin >> choice;
    if (std::cin && choice < 3 && choice >= 0)
    {
        return static_cast<Choice>(choice);
    }
    return {};
}

Choice computer_turn()
{
	// gen random number
	// dist .. uniform 1/3, 1/3, 1/3
	static std::mt19937 gen{ std::random_device{}() };
    static std::uniform_int_distribution dist(0, 2);
    int choice = dist(gen);
    return static_cast<Choice>(choice);
}

constexpr bool first_player_wins(Choice first, Choice second)
{
    return first == Choice::Rock && second == Choice::Scissors
    || first == Choice::Paper && second == Choice::Rock
    || first == Choice::Scissors && second == Choice::Paper;
}

void check_properties()
{
    static_assert(!first_player_wins(Choice::Rock, Choice::Rock));
    static_assert(!first_player_wins(Choice::Rock, Choice::Paper));
    static_assert(first_player_wins(Choice::Rock, Choice::Scissors));

    static_assert(first_player_wins(Choice::Paper, Choice::Rock));
    static_assert(!first_player_wins(Choice::Paper, Choice::Paper));
    static_assert(!first_player_wins(Choice::Paper, Choice::Scissors));
    
    static_assert(!first_player_wins(Choice::Scissors, Choice::Rock));
    static_assert(first_player_wins(Choice::Scissors, Choice::Paper));
    static_assert(!first_player_wins(Choice::Scissors, Choice::Scissors));
}

void play()
{
    std::cout << "Rock(0), Paper(1), Scissors(2)\n";
    int turns{};
    int human_won{};
    int computer_won{};
    while (true)
    {
        auto human = human_turn();
        if (!human)
        {
            break;
        }
        ++turns;
        auto computer = computer_turn(); // TODO
        std::cout << *human << " v. " << computer << '\n';
        if (first_player_wins(human.value(), computer))
        {
            std::cout << "You chose: " << *human << ". You won!\n";
            ++human_won;
        }
        if (first_player_wins(computer, human.value()))
        {
            std::cout << "You chose: " << *human << ". You lost!\n";
            ++computer_won;
        }
    }
    std::cout << "You won " << human_won << " v. computer won " << computer_won << '\n';
}

int main(int argc, char* argv[])
{
    std::cout << "start!";
    play();
}
