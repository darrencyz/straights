#include "CardGui.h"
#include <string>

// String constants used to create a file name
const std::string IMG_FOLDER = "img/";
const std::string EXTENSION = ".png";
const std::string EMPTY_CARD_IMG = "nothing";

// ****************
// Helper functions
// ****************

// Directly convert an integer to a string representation
// instead of using another C/C++ library
std::string intToStr(int val) {
    switch(val) {
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        case 10:
            return "j";
        case 11:
            return "q";
        case 12:
            return "k";
        default:
            return "";
    }
}

// Given two numbers, create a file name
// ex) 0_0.png is ace of club
std::string convertToFilename(int firstNum, int secondNum) {

    std::string filename = IMG_FOLDER 
                        + intToStr(firstNum) + "_"
                        + intToStr(secondNum)
                        + EXTENSION;
    return filename;
}

// Create Pixbuf from an image file
Glib::RefPtr<Gdk::Pixbuf> createPixbuf(const std::string& name) {
    return Gdk::Pixbuf::create_from_file(name);
}

// ***********************
// End of helper functions
// ***********************

// Constructor
CardGui::CardGui() {
    
    // Create pixbufs for all image files and store them in a 2D array
    for (int i = 0; i < SUIT_COUNT; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            std::string filename = convertToFilename(i, j);
            imgArray_[i][j] = createPixbuf(filename);
        }
    }

    // Separate pixbuf for an empty card
    std::string emptyCardName = IMG_FOLDER + EMPTY_CARD_IMG + EXTENSION;
    emptyCardImg_ = createPixbuf(emptyCardName);
}

CardGui::~CardGui() {

}

// Accessor for a pixbuf of an image file
Glib::RefPtr<Gdk::Pixbuf> CardGui::getCardImg(Suit s, Rank r) {
    return imgArray_[(int) s][(int) r];
}

// Accessor for an empty card
Glib::RefPtr<Gdk::Pixbuf> CardGui::getEmptyCardImg() {
    return emptyCardImg_;
}

