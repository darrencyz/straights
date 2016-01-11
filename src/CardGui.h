#ifndef CARDGUI_H
#define CARDGUI_H

#include <gdkmm/pixbuf.h>
#include "Card.h"

#define SUIT_COUNT 4
#define RANK_COUNT 13

class Card;

class CardGui {
public:
    CardGui();
    ~CardGui();

    // Accessors
    Glib::RefPtr<Gdk::Pixbuf> getCardImg(Suit s, Rank r);
    Glib::RefPtr<Gdk::Pixbuf> getEmptyCardImg();

private:
    // 2D array for pixbufs of all card images
    Glib::RefPtr<Gdk::Pixbuf> imgArray_[SUIT_COUNT][RANK_COUNT];

    // pixbuf for an empty card image
    Glib::RefPtr<Gdk::Pixbuf> emptyCardImg_;
};

#endif

