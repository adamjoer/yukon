#pragma once

#include "linked_list.h"

enum FileValidationStatus {
    OK,
    FileNotFound,
    InvalidFormat,
    InvalidRank,
    InvalidSuit,
    InvalidSuitCardCount,
    InvalidRankCardCount
};

enum Command {
    InvalidInputFormat,
    Error,
    Play,
    QuitGame,
    QuitProgram,
    LoadFile,
    ShowCards,
    ShuffleSplit,
    ShuffleRandom,
    SaveDeck,
    MoveCard
};

extern char moved_card[];
extern char source_column[];
extern char destination_column[];
extern char argument[];

void load_from_file(LinkedList *list, const char *filepath, bool check_file);

enum FileValidationStatus validate_file(const char *filepath);

enum Command get_user_command();

int get_card_value(char rank);

bool is_valid_column(const char *string);

bool is_valid_foundation(const char *string);

bool is_valid_card(const char *string);

void save_deck_to_file(LinkedList *list, const char *filepath);
