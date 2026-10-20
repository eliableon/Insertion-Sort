#pragma once

#include <random>
#include <string>
#include <vector>

class BayesBeatGame {
public:
    BayesBeatGame();
    void run();

private:
    struct Tool {
        std::string name;
        std::vector<double> effectiveness;

        // Evidence learned from every color.
        int totalSuccesses = 0;
        int totalFailures = 0;
    };

    struct Block {
        std::string color;
    };

    struct Belief {
        std::string toolName;
        std::string color;
        int successes;
        int failures;

        double probability() const;
    };

    std::vector<std::string> colors_;
    std::vector<Tool> tools_;
    std::vector<Belief> beliefs_;
    std::vector<std::string> observations_;

    std::mt19937 randomGenerator_;

    void createTools();
    void createInitialBeliefs();

    std::vector<Block> createBlocks();
    void displayTools() const;
    void displayBlocks(const std::vector<Block>& blocks) const;
    void displayBeliefsForColor(const std::string& color) const;
    void displayObservations() const;

    int readChoice(int minimum, int maximum) const;
    int findBelief(
        const std::string& toolName,
        const std::string& color) const;
    int findTool(const std::string& toolName) const;

    // Uses color-specific evidence when available.
    // Otherwise, uses evidence transferred from other colors.
    double probabilityFor(
        const std::string& toolName,
        const std::string& color) const;

    int chooseBestTool(const std::string& color) const;
    bool destroyBlock(
        const Tool& tool,
        const std::string& color);

    void updateBelief(
        const std::string& toolName,
        const std::string& color,
        bool success);

    void explainDecision(
        const std::string& color,
        const std::string& selectedTool,
        double probabilityBefore,
        double probabilityAfter,
        bool success) const;
};