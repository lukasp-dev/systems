/**
Implement a BowlingScoreSheet class that tracks rolls in a
ten-pin bowling game and returns frame-level scores.

Requirements

The class should expose:

void onBallRolled(int pins)

Appends a roll to the game after validating that pins is
between 0 and 10 inclusive.

std::pair<int, int> getFrameScore(int frame) const

Returns:

{ frame_score, running_total_through_frame }

where frame is 1-indexed from 1 to 10.

Scoring Rules

- Open frame:
    score = first roll + second roll

- Spare:
    score = 10 + next roll

- Strike:
    score = 10 + next two rolls

- Frames 1-9 advance by:
    - 1 roll for a strike
    - 2 rolls otherwise

- Frame 10 may include bonus rolls for a spare or strike.

- Missing rolls or missing bonus rolls should count as 0.
 */
#include <bits/stdc++.h>
using namespace std;

class BowlingScoreSheet {
private:
    vector<int> rolls;

    int getRoll(int idx) const {
        if (idx >= static_cast<int>(rolls.size())) {
            return 0;
        }

        return rolls[idx];
    }

public:
    void onBallRolled(int pins) {
        if (pins < 0 || pins > 10) {
            throw invalid_argument("bad input");
        }

        rolls.push_back(pins);
    }

    // {frame_score, running_total_through_frame}
    pair<int, int> getFrameScore(int frame) const {
        if (frame < 1 || frame > 10) {
            throw invalid_argument("frame must be between 1 and 10");
        }

        int runningTotal = 0;
        int rollIdx = 0;

        for (int frameNum = 1; frameNum <= frame; ++frameNum) {
            int frameScore = 0;

            // Strike
            if (getRoll(rollIdx) == 10) {
                frameScore =
                    10
                    + getRoll(rollIdx + 1)
                    + getRoll(rollIdx + 2);

                rollIdx += 1;
            }

            // Spare or open frame
            else {
                int first = getRoll(rollIdx);
                int second = getRoll(rollIdx + 1);

                // Spare
                if (first + second == 10) {
                    frameScore =
                        10
                        + getRoll(rollIdx + 2);
                }

                // Open
                else {
                    frameScore = first + second;
                }

                rollIdx += 2;
            }

            runningTotal += frameScore;

            if (frameNum == frame) {
                return {frameScore, runningTotal};
            }
        }

        return {0, 0};
    }
};