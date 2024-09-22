#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <vector>

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

auto lookup_to_weighted_distrib(const std::vector<Choice> & choices)
{
    std::map<Choice, int> frequencies;
    for (auto choice: choices)
    {
        ++frequencies[choice];
    }
    // std::discrete_distribution <- 0, 1, 2
    std::vector<int> counts{ frequencies[Choice::Rock], frequencies[Choice::Paper], frequencies[Choice::Scissors] };
    // 0, 1, 2
    return std::discrete_distribution(counts.begin(), counts.end());
}

Choice wins_against(Choice choice)
{
    if (choice == Choice::Rock)
    {
        return Choice::Paper;
    }
    else if (choice == Choice::Paper)
    {
        return Choice::Scissors;
    }
    return Choice::Rock;
    
}

class Learner
{
    // map tuple(computer human) -> vector<Choice>
    std::map<std::tuple<Choice, Choice>, std::vector<Choice>> lookup;
    std::optional<Choice> previous_computer_turn{};
    std::optional<Choice> previous_human_turn{};

    Choice last_computer_turn;

    std::mt19937 gen{ std::random_device{}() };
    public:
        Choice turn()
        {
            // Rock v Paper -> Scissors, Scissors, Paper
            // 0, 1/3, 2/3
            if (previous_computer_turn && previous_human_turn)
            {
                auto it = lookup.find({previous_computer_turn.value(), previous_human_turn.value()});
                if (it != lookup.end())
                {
                    // predict
                    auto dist = lookup_to_weighted_distrib(it->second);
                    int likely_human_choice = dist(gen);
                    last_computer_turn = wins_against(static_cast<Choice>(likely_human_choice));
                    return last_computer_turn;
                    // std::discrete_distribution <- 0, 1, 2
                }
                
            }
            last_computer_turn = computer_turn();
            return last_computer_turn;
        }

    void update(Choice human_turn)
        {
            if (previous_computer_turn && previous_human_turn)
            {
                lookup[{previous_computer_turn.value(), previous_human_turn.value()}].push_back(human_turn);
            }
            previous_computer_turn = last_computer_turn;
            previous_human_turn = human_turn;
        }
};

void play_learner()
{
    std::cout << "Rock(0), Paper(1), Scissors(2)\n";
    int turns{};
    int human_won{};
    int computer_won{};
    Learner learner;
    while (true)
    {
        auto human = human_turn();
        if (!human)
        {
            break;
        }
        ++turns;
        auto computer = learner.turn();
        learner.update(human.value());
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
    play_learner();
}
