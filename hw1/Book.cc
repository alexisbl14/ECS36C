#include "Book.h"

//Book constructor
Book::Book(int ISBN, std::string language, std::string condition)
{
    this->ISBN = ISBN;
    this->language = language;
    this->condition = condition;
}

//allows us to get ISBN
int Book::getISBN()
{
    return this->ISBN;
}

//allows us to get language
std::string Book::getlanguage()
{
    return this->language;
}

//allows us to get condition
int Book::getcondition()
{
    int condition_value;
    if ((this->condition) == "new")  //define new condition as 1 for operator overloading
    {
        condition_value = 1;
    }
    if ((this->condition) == "digital") //define digital condition as 2 for operator overloading
    {
        condition_value = 2;
    }
    if ((this->condition) == "used") //define digital condition as 3 for operator overloading
    {
        condition_value = 3;
    }
    return condition_value;
}

//operator overloading to compare 2 books
bool Book::operator<(Book &Book)
{
    if(this->getISBN() != Book.getISBN())  //first compare ISBN
    {
        return (this->getISBN() < Book.getISBN());
    }
    else                                            //if ISBN same, compare language reverse alphabetically
    {
        if(this->getlanguage() != Book.getlanguage())
        {
            return(this->getlanguage() > Book.getlanguage());
        }        
        else                                      //if ISBN and language the same, compare condition
        {
            //check new < digital < used
            return(this->getcondition() < Book.getcondition());
        }   
        return false;
    }
}

bool Book::operator==(Book &Book)
{
    //check if all components of book are equal
    if((this->getISBN() == Book.getISBN()) && (this->getlanguage() == Book.getlanguage()) 
                                            && (this->getcondition() == Book.getcondition()))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}