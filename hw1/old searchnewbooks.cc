/*std::string line;                   //declaring variables to use later
    std::string tempISBN, lang, condition;
    int ISBN;*/

    //parse_line(argv[1], new_books);
    /*std::ifstream newBooksFile(argv[1]);//opening file with the name of the second argument
    if(newBooksFile.fail())
    {
        std::cerr << "Error: cannot open file " << argv[1] << endl;
        return 0;
    }
    while(getline(newBooksFile, line))  //reading file line by line
    {
        std::stringstream ss(line);   //using stringstream to parse line into variables
        getline(ss, tempISBN, ',');
        getline(ss, lang, ',');
        getline(ss, condition, ',');
        ISBN = stoi(tempISBN);
        new_books.push_back(Book(ISBN, lang, condition)); //creating a book object and adding it to vector
    }
    newBooksFile.close();  //closing file*/

    
    //parse_line(argv[2], request_books);

    /*std::ifstream requestBooksFile(argv[2]); //opening file with name of third argument
    if(requestBooksFile.fail())
    {
        std::cerr << "Error: cannot open file " << argv[2] << endl;
        return 0;
    }                                             
    while(getline(requestBooksFile, line))  //reads file in line by line
    {
        std::stringstream ss(line);     //using string stream to parse line into variables
        getline(ss, tempISBN, ',');
        getline(ss, lang, ',');
        getline(ss, condition, ',');
        ISBN = stoi(tempISBN);
        request_books.push_back(Book(ISBN, lang, condition));  //creating book object and adding it to vector
    }
    requestBooksFile.close();  //closing file*/

 //   (target.getISBN() == book_vector[m].getISBN()) && (target.getlanguage() == book_vector[m].getlanguage())
  //                                                          && (target.getcondition() == book_vector[m].getcondition())
