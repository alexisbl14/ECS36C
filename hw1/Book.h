#include <iostream>
#ifndef BOOK_H
#define BOOK_H
class Book
{
    private:
        int ISBN;
        std::string language;
        std::string condition;
        
        
    public:
        Book(int ISBN, std::string language, std::string condition);        
        int getISBN();
        std::string getlanguage();
        int getcondition();
        bool operator<(Book &Book);
        bool operator==(Book &Book);
};
#endif