#include "BayesGame.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

BayesBeatGame::BayesBeatGame()
    : colors_{ "Red", "Blue", "Green", "Yellow" },
    randomGenerator_(std::random_device{}()) {
    createTools();
    createInitialBeliefs();
}

double BayesBeatGame::Belief::probability() const {
    // Beta prior/posterior estimate:
    //
    // P(tool works | evidence) =
    // (successes + 1) / (successes + failures + 2)
    //
    // The +1 values represent a neutral prior belief.
    return static_cast<double>(successes + 1) /
        static_cast<double>(successes + failures + 2);
}

void BayesBeatGame::createTools() {
    tools_.clear();

    std::vector<std::string> possibleTools{
        "Hammer",
        "Laser",
        "Drill",
        "Freezer",
        "Acid Spray"
    };

    std::shuffle(
        possibleTools.begin(),
        possibleTools.end(),
        randomGenerator_);

    std::uniform_real_distribution<double> effectivenessDistribution(0.35, 0.95);

    for (int i = 0; i < 3; ++i) {
        Tool tool;
        tool.name = possibleTools[i];

        for (std::size_t colorIndex = 0;
            colorIndex < colors_.size();
            ++colorIndex) {
            tool.effectiveness.push_back(effectivenessDistribution(randomGenerator_));
        }

        tools_.push_back(tool);
    }
}

void BayesBeatGame::createInitialBeliefs() {
    beliefs_.clear();

    for (const Tool& tool : tools_) {
        for (const std::string& color : colors_) {
            // successes = 0 and failures = 0 gives a 50% neutral prior.
            beliefs_.push_back({ tool.name, color, 0, 0 });
        }
    }
}

std::vector<BayesBeatGame::Block> BayesBeatGame::createBlocks() {
    std::vector<Block> blocks;

    std::uniform_int_distribution<int> colorDistribution(
        0,
        static_cast<int>(colors_.size()) - 1);

    for (int i = 0; i < 3; ++i) {
        blocks.push_back({ colors_[colorDistribution(randomGenerator_)] });
    }

    return blocks;
}

void BayesBeatGame::displayTools() const {
    std::cout << "\nAvailable tools:\n";

    for (std::size_t i = 0; i < tools_.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << tools_[i].name << '\n';
    }
}

void BayesBeatGame::displayBlocks(
    const std::vector<Block>& blocks) const {
    std::cout << "\nBlocks currently available:\n";

    for (std::size_t i = 0; i < blocks.size(); ++i) {
        std::cout << "  Position " << i + 1
            << ": " << blocks[i].color << " block\n";
    }
}

int BayesBeatGame::readChoice(int minimum, int maximum) const {
    int choice;

    while (true) {
        std::cin >> choice;

        if (!std::cin.fail() && choice >= minimum && choice <= maximum) {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n');
            return choice;
        }

        std::cout << "Please enter a number from "
            << minimum << " to " << maximum << ": ";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');
    }
}

int BayesBeatGame::findBelief(
    const std::string& toolName,
    const std::string& color) const {
    for (std::size_t i = 0; i < beliefs_.size(); ++i) {
        if (beliefs_[i].toolName == toolName &&
            beliefs_[i].color == color) {
            return static_cast<int>(i);
        }
    }

    return -1;
}
int BayesBeatGame::findTool(
    const std::string& toolName) const {
    for (std::size_t i = 0; i < tools_.size(); ++i) {
        if (tools_[i].name == toolName) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

double BayesBeatGame::probabilityFor(
    const std::string& toolName,
    const std::string& color) const {
    int beliefIndex = findBelief(
        toolName,
        color);

    if (beliefIndex == -1) {
        // No evidence exists for this tool/color combination.
        return 0.5;
    }

    // Only evidence for this exact color is used.
    return beliefs_[beliefIndex].probability();
}

void BayesBeatGame::displayBeliefsForColor(
    const std::string& color) const {
    std::cout << "\nProbability estimates for "
        << color << " blocks:\n";

    std::cout << std::fixed << std::setprecision(1);

    for (const Tool& tool : tools_) {
        int beliefIndex = findBelief(
            tool.name,
            color);

        const Belief& belief = beliefs_[beliefIndex];

        std::cout << "  " << tool.name
            << ": " << belief.probability() * 100.0 << "% "
            << "(successes: " << belief.successes
            << ", failures: " << belief.failures
            << ")\n";
    }

    std::cout << "\nOnly evidence involving "
        << color << " blocks is used for these predictions.\n";
}

int BayesBeatGame::chooseBestTool(
    const std::string& color) const {
    int bestToolIndex = 0;
    double highestProbability = -1.0;

    for (std::size_t i = 0; i < tools_.size(); ++i) {
        double probability = probabilityFor(
            tools_[i].name,
            color);

        if (probability > highestProbability) {
            highestProbability = probability;
            bestToolIndex = static_cast<int>(i);
        }
    }

    return bestToolIndex;
}

bool BayesBeatGame::destroyBlock(
    const Tool& tool,
    const std::string& color) {
    std::size_t colorIndex = 0;

    for (std::size_t i = 0; i < colors_.size(); ++i) {
        if (colors_[i] == color) {
            colorIndex = i;
            break;
        }
    }

    std::bernoulli_distribution outcome(
        tool.effectiveness[colorIndex]);

    return outcome(randomGenerator_);
}

void BayesBeatGame::updateBelief(
    const std::string& toolName,
    const std::string& color,
    bool success) {
    int beliefIndex = findBelief(
        toolName,
        color);

    if (beliefIndex == -1) {
        return;
    }

    if (success) {
        ++beliefs_[beliefIndex].successes;
    }
    else {
        ++beliefs_[beliefIndex].failures;
    }
}

void BayesBeatGame::explainDecision(
    const std::string& color,
    const std::string& selectedTool,
    double probabilityBefore,
    double probabilityAfter,
    bool success) const {
    std::cout << "\nAI decision explanation:\n";
    std::cout << "  Target block: " << color << '\n';
    std::cout << "  Selected tool: " << selectedTool << '\n';
    std::cout << "  Probability before evidence: "
        << probabilityBefore * 100.0 << "%\n";
    std::cout << "  New evidence: "
        << (success ? "The block was destroyed."
            : "The block resisted the tool.")
        << '\n';
    std::cout << "  Probability after evidence: "
        << probabilityAfter * 100.0 << "%\n";

    if (success) {
        std::cout << "  Learning result: this tool becomes more likely "
            "to work on this color in the future.\n";
    }
    else {
        std::cout << "  Learning result: this tool becomes less likely "
            "to work on this color in the future.\n";
    }
}

void BayesBeatGame::displayObservations() const {
    std::cout << "\nStored observations and evidence:\n";

    if (observations_.empty()) {
        std::cout << "  No observations recorded yet.\n";
        return;
    }

    for (std::size_t i = 0; i < observations_.size(); ++i) {
        std::cout << "  " << i + 1 << ". "
            << observations_[i] << '\n';
    }
}

void BayesBeatGame::run() {
    constexpr int totalRounds = 5;

    std::cout << "\n====================================\n";
    std::cout << "          BAYES BEAT GAME\n";
    std::cout << "====================================\n";
    std::cout << "The game begins with a neutral 50% prior belief.\n";
    std::cout << "Each success or failure is new evidence.\n";
    std::cout << "The probability estimates are updated after every round.\n";

    displayTools();

    for (int round = 1; round <= totalRounds; ++round) {
        std::cout << "\n------------------------------------\n";
        std::cout << "Round " << round << " of " << totalRounds << '\n';

        std::vector<Block> blocks = createBlocks();
        displayBlocks(blocks);

        std::cout << "\nChoose a block position: ";
        int position = readChoice(1, static_cast<int>(blocks.size()));

        const std::string& selectedColor = blocks[position - 1].color;

        displayBeliefsForColor(selectedColor);

        int recommendedToolIndex = chooseBestTool(selectedColor);

        std::cout << "\nThe AI currently recommends: "
            << tools_[recommendedToolIndex].name << '\n';

        displayTools();
        std::cout << "Choose a tool: ";
        int toolChoice = readChoice(1, static_cast<int>(tools_.size()));

        Tool& selectedTool = tools_[toolChoice - 1];

        double probabilityBefore =
            probabilityFor(
                selectedTool.name,
                selectedColor);

        bool success = destroyBlock(
            selectedTool,
            selectedColor);

        updateBelief(
            selectedTool.name,
            selectedColor,
            success);

        double probabilityAfter =
            probabilityFor(
                selectedTool.name,
                selectedColor);

        std::string observation =
            "Round " + std::to_string(round) +
            ": " + selectedTool.name +
            " used on " + selectedColor +
            " block - " +
            (success ? "SUCCESS" : "FAILURE");

        observations_.push_back(observation);

        std::cout << "\nResult: "
            << (success ? "SUCCESS! The block was destroyed."
                : "FAILURE! The block survived.")
            << '\n';

        explainDecision(
            selectedColor,
            selectedTool.name,
            probabilityBefore,
            probabilityAfter,
            success);

        displayObservations();

        std::cout << "\nUpdated knowledge for "
            << selectedColor << " blocks:\n";
        displayBeliefsForColor(selectedColor);
    }

    std::cout << "\n====================================\n";
    std::cout << "Game complete. The AI's learned knowledge is:\n";
    displayObservations();

    std::cout << "\nThe next time a color appears, the AI can use "
        "these updated probabilities as prior knowledge.\n";
}