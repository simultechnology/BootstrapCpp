#include <array>
#include <iostream>
#include <optional>
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
    // TODO - make random
    return Choice::Rock;
}

void play()
{
    std::cout << "Rock(0), Paper(1), Scissors(2)\n";
    while (true)
    {
        auto human = human_turn();
        std::cout << "You chose: " << *human << "\n";
        if (!human)
        {
            break;
        }
        auto computer = computer_turn(); // TODO
        std::cout << *human << " v. " << computer << '\n';
    }
}

int main(int argc, char* argv[])
{
    std::cout << "start!";
    play();
}
