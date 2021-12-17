#include <iostream>
#include <stdio.h>
#include <strings.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono> // NOLINT
#include "Book.h"

using namespace std;

int parse_line(std::string filename, std::vector<Book> &book_vector);

bool linear_search(Book target, std::vector<Book> &book_vector);

bool binary_search(Book target, std::vector<Book> &book_vector, int l, int r);

int main(int argc, char** argv) //allows us to read in multiple arguments
{
    if(argc != 4)
    {
        std::cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>" << endl;
        exit(1);
    }

    std::vector<Book > new_books; //creating vectors for new books and requested books
    std::vector<Book > request_books;   

    
    parse_line(argv[1], new_books); //parsing newbooks.dat
    
    parse_line(argv[2], request_books); //parsing request.dat        
    

    std::string search_type;
    std::cout << "Choice of search method ([l]inear, [b]inary)?" << std::endl; //ask user for search type
    getline(cin, search_type);

    std::chrono::high_resolution_clock::time_point start;

    
    while(search_type != "l" && search_type != "b")  //check to see if input is valid
    {
        std::cerr << "Incorrect choice" << std::endl;
        getline(cin, search_type);
    } 

    int count = 0;  //count of books that match

    if (search_type == "l")
    {
    //linear search
        start = std::chrono::high_resolution_clock::now();
        unsigned int i;
        for (i = 0; i < request_books.size(); i++)  //iterating through each book in request.dat to see if it is in newbooks.dat
        {
            if (linear_search(request_books[i], new_books))
            {
                count += 1;
            }
        }
    }
    else
    {
        //binary search
        start = std::chrono::high_resolution_clock::now();
        unsigned int i;
        std::sort(new_books.begin(), new_books.end());  //sort books in newbooks first
        for (i = 0; i < request_books.size(); i++)  //iterating through each book in request.dat to see if it is in newbooks.dat
        {
            if (binary_search(request_books[i], new_books, 0, (new_books.size()-1)))
            {
                count += 1;
            }
        }

    }
    auto end = std::chrono::high_resolution_clock::now();

    double elapsed_us = std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << "CPU time: " << elapsed_us << " microseconds" << endl;  

    ofstream resultFile(argv[3]);
    if(resultFile.fail())
    {
        std::cerr << "Error: cannot open file " << argv[3] << endl;
        exit(1);
    } 
    resultFile << count << "\n";
    resultFile.close();
}

bool linear_search(Book target, std::vector<Book> &book_vector)
{
    unsigned int i;
    for (i = 0; i < book_vector.size(); i++)  //iterating through newbooks vector
    {
        //if target matches a book in new_books, return true
        if (target == book_vector[i])
        {
            return true;
        }
    }
    return false;   
}
bool binary_search(Book target, std::vector<Book> &book_vector, int l, int r)
{
    unsigned int i, j;
    i = l;                  //i is leftmost index, j is right index
    j = r;
    while(i < j)
    {
        int m = (i + j)/2;    //m is the middle
        if(book_vector[m] < target)  //if target is greater than book[m], go right
        {
            i = m+1;
        }
        else                        //if target is less than or equal to, move left
        {
            j = m;
        }
        if(target == book_vector[i])    //if target matches book[i], return true
        {
            return true;
        }
    }
    return false;   
}

int parse_line(std::string filename, std::vector<Book> &book_vector)
{
    
    std::string line;
    std::string tempISBN, lang, condition;
    int ISBN;
    std::ifstream BooksFile(filename); //opening file with the name of the second argument
    if(BooksFile.fail())    //check to see if file can open, if not, throw an exception to catch
    {
        std::cerr << "Error: cannot open file " << filename << endl;
        exit (1);
    }
    while(getline(BooksFile, line))  //reading file line by line
    {
        std::stringstream ss(line);
        getline(ss, tempISBN, ',');
        getline(ss, lang, ',');
        getline(ss, condition, ',');
        ISBN = stoi(tempISBN);
        Book b1 = Book(ISBN, lang, condition);
        book_vector.push_back(b1);
    }
    BooksFile.close();   //closing file

    return 0;    
}
