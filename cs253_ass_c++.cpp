#include <bits/stdc++.h>
using namespace std;

class Book
{
public:
    string Title;
    string Author;
    string ISBN;
    string Publication;
    string Availability;
    int DueTime;
    int LastIssueTime;
    int LastReturnTime;
};

class User
{
public:
    string Name;
    string ID;
    string Password;
    string UserType;
    int number_of_book_issued;
    vector<Book> ListOfBookIssued;
};

class BookDatabase : public Book
{
public:
    vector<Book> ListOfBooks;
    void Add(string title, string author, string isbn, string publication, string availability, BookDatabase *bookDatabase)
    {

        Book Book1;
        Book1.Title = title;
        Book1.Author = author;
        Book1.Publication = publication;
        Book1.ISBN = isbn;
        Book1.Availability = availability;

        bookDatabase->ListOfBooks.push_back(Book1);
    };

    void Update(string new_title, string new_author, string isbn, string new_publication, string new_availability, BookDatabase *bookDatabase)
    {
        for (int i = 0; i < bookDatabase->ListOfBooks.size(); i++)
        {
            if (bookDatabase->ListOfBooks[i].ISBN == isbn)
            {
                bookDatabase->ListOfBooks[i].Title = new_title;
                bookDatabase->ListOfBooks[i].Author = new_author;
                bookDatabase->ListOfBooks[i].Publication = new_publication;
                bookDatabase->ListOfBooks[i].Availability = new_availability;
            }
        }
    };

    void Delete(string isbn, BookDatabase *bookDatabase)
    {
        for (int i = 0; i < bookDatabase->ListOfBooks.size(); i++)
        {
            if (bookDatabase->ListOfBooks[i].ISBN == isbn)
            {
                bookDatabase->ListOfBooks.erase(ListOfBooks.begin() + i);
            }
        }
    };
    void Search(string isbn, BookDatabase *bookDatabase)
    {
        for (int i = 0; i < bookDatabase->ListOfBooks.size(); i++)
        {
            if (bookDatabase->ListOfBooks[i].ISBN == isbn)
            {
                cout << bookDatabase->ListOfBooks[i].Title << endl;
                cout << bookDatabase->ListOfBooks[i].Author << endl;
                cout << bookDatabase->ListOfBooks[i].ISBN << endl;
                cout << bookDatabase->ListOfBooks[i].Publication << endl;
                cout << bookDatabase->ListOfBooks[i].Availability << endl;
            }
        }
    };
    // friend class Librarian;
};

class UserDatabase : public User
{
public:
    vector<User> ListOfUsers;

    int validId(string id, UserDatabase *userDatabase)
    {
        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                return 1;
            }
        }
        return 0;
    }

    int validPassword(string id, string password, UserDatabase *userDatabase)
    {
        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                if (userDatabase->ListOfUsers[i].Password == password)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        return 0;
    }

    void ReturnBook(string isbn, string id, BookDatabase *bookDatabase, UserDatabase *userDatabase)
    {
        cout << "you have successfully returned the book with id : " << id << endl;
        cout << flush;

        time_t return_time = time(0);
        tm *ltm = localtime(&return_time);
        bookDatabase->LastReturnTime = return_time;
        cout << "you are returning the book on (D M Y) : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
        cout << flush;

        int fine = 0;

        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                for (int j = 0; j < userDatabase->ListOfUsers[i].ListOfBookIssued.size(); j++)
                {
                    if (userDatabase->ListOfUsers[i].ListOfBookIssued[j].ISBN == isbn)
                    {
                        if (userDatabase->UserType == "student")
                        {
                            fine = ((return_time - userDatabase->ListOfUsers[i].ListOfBookIssued[j].DueTime) / (24 * 60 * 60)) * 2;
                        }
                        else if (userDatabase->UserType == "professor")
                        {
                            fine = ((return_time - userDatabase->ListOfUsers[i].ListOfBookIssued[j].DueTime) / (24 * 60 * 60)) * 5;
                        }
                        cout << "you have to pay " << fine << " rupee fine for this book" << endl;
                        cout << flush;

                        userDatabase->ListOfUsers[i].ListOfBookIssued.erase(userDatabase->ListOfUsers[i].ListOfBookIssued.begin() + j);
                    }
                }
            }
        }

        bookDatabase->Availability = "is available";
    };

    void BookRequestStudent(string isbn, string id, BookDatabase *bookDatabase, UserDatabase *userDatabase)
    {
        char issue;
        for (int i = 0; i < bookDatabase->ListOfBooks.size(); i++)
        {
            if (bookDatabase->ListOfBooks[i].ISBN == isbn)
            {
                if (bookDatabase->ListOfBooks[i].Availability == "is available")
                {
                    cout << "Do you want to issue this book? (y/n): " << endl;
                    cout << flush;
                    cin >> issue;
                    if (issue == 'y')
                    {
                        for (int j = 0; j < userDatabase->ListOfUsers.size(); j++)
                        {
                            if (userDatabase->ListOfUsers[j].ID == id)
                            {
                                if (userDatabase->ListOfUsers[j].number_of_book_issued < 5)
                                {
                                    time_t due_time = time(0) + 30 * 24 * 60 * 60;
                                    tm *ltm = localtime(&due_time);
                                    userDatabase->ListOfUsers[j].number_of_book_issued = userDatabase->ListOfUsers[i].number_of_book_issued + 1;
                                    userDatabase->ListOfUsers[j].ListOfBookIssued.push_back(bookDatabase->ListOfBooks[i]);
                                    userDatabase->ListOfUsers[j].ListOfBookIssued[userDatabase->ListOfUsers[j].ListOfBookIssued.size() - 1].DueTime = due_time;
                                    userDatabase->ListOfUsers[j].ListOfBookIssued[userDatabase->ListOfUsers[j].ListOfBookIssued.size() - 1].LastIssueTime = due_time - 24 * 60 * 60 * 30;
                                    cout << "You have issued this book, you have to return it by  (D M Y) : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
                                    cout << flush;
                                    bookDatabase->ListOfBooks[i].Availability = "not available";
                                    return;
                                }
                                else
                                {
                                    cout << "you already have issued maximum number of books." << endl;
                                    cout << flush;
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
        cout << "sorry, the book is not available. It has been issued by someone." << endl;
        cout << flush;
        return;
    };

    void BookRequestProf(string isbn, string id, BookDatabase *bookDatabase, UserDatabase *userDatabase)
    {
        char issue;
        for (int i = 0; i < bookDatabase->ListOfBooks.size(); i++)
        {
            if (bookDatabase->ListOfBooks[i].ISBN == isbn)
            {
                if (bookDatabase->ListOfBooks[i].Availability == "is available")
                {
                    cout << "Do you want to issue this book? (y/n): " << endl;
                    cout << flush;
                    cin >> issue;
                    if (issue == 'y')
                    {
                        for (int j = 0; j < userDatabase->ListOfUsers.size(); j++)
                        {
                            if (userDatabase->ListOfUsers[j].ID == id)
                            {

                                time_t due_time = time(0) + 60 * 24 * 60 * 60;
                                tm *ltm = localtime(&due_time);
                                userDatabase->ListOfUsers[j].number_of_book_issued = userDatabase->ListOfUsers[i].number_of_book_issued + 1;
                                userDatabase->ListOfUsers[j].ListOfBookIssued.push_back(bookDatabase->ListOfBooks[i]);
                                userDatabase->ListOfUsers[j].ListOfBookIssued[userDatabase->ListOfUsers[j].ListOfBookIssued.size() - 1].DueTime = due_time;
                                userDatabase->ListOfUsers[j].ListOfBookIssued[userDatabase->ListOfUsers[j].ListOfBookIssued.size() - 1].LastIssueTime = due_time - 24 * 60 * 60 * 60;
                                cout << "You have issued this book, you have to return it by  (D M Y) : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
                                cout << flush;
                                bookDatabase->ListOfBooks[i].Availability = "not available";
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "sorry, the book is not available. It has been issued by someone." << endl;
        cout << flush;
        return;
    };

    void Add(string name, string id, string password, string userType, UserDatabase *userDatabase)
    {
        User a;
        a.Name = name;
        a.ID = id;
        a.Password = password;
        a.UserType = userType;
        a.number_of_book_issued = 0;
        userDatabase->ListOfUsers.push_back(a);
    };
    void Update(string id, string new_name, string new_password, string new_userType, UserDatabase *userDatabase)
    {
        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                userDatabase->ListOfUsers[i].Name = new_name;
                if (new_password != "master")
                {
                    userDatabase->ListOfUsers[i].Password = new_password;
                }
                userDatabase->ListOfUsers[i].UserType = new_userType;
            }
        }
    };
    void Delete(string id, UserDatabase *userDatabase)
    {
        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                userDatabase->ListOfUsers.erase(ListOfUsers.begin() + i);
            }
        }
    };
    User Search(string id, UserDatabase *userDatabase)
    {
        for (int i = 0; i < userDatabase->ListOfUsers.size(); i++)
        {
            if (userDatabase->ListOfUsers[i].ID == id)
            {
                return userDatabase->ListOfUsers[i];
            }
        }
        return userDatabase->ListOfUsers[0];
    };
};

int main()
{
    int exit=0;
    UserDatabase userDatabase;
    BookDatabase bookDatabase;
    while (exit != 1)
    {
        cout << "Welcome to the IITK E-Library." << endl;
        cout << flush;
        cout << "Are you a new User? (y/n) : " << endl;
        cout << flush;
        char new_user;
        cin >> new_user;
        if (new_user == 'y')
        {
            string new_user_name;
            string new_user_id;
            string new_user_password;
            string new_user_type;
            cout << "Welcome to the registraion page." << endl;
            cout << flush;
            cout << "enter your name : " << endl;
            cout << flush;
            cin >> new_user_name;
            cout << endl
                 << "enter your id : " << endl;
            cout << flush;
            cin >> new_user_id;

            cout << "enter your Password : " << endl;
            cout << flush;
            cin >> new_user_password;

            cout << "You are a student/professor/librarian : " << endl;
            cout << flush;
            cin >> new_user_type;
            userDatabase.Add(new_user_name, new_user_id, new_user_password, new_user_type, &userDatabase);
            cout << "You have been registered to IITK E-Library. Please login again with your Credentials." << endl;
            cout << flush;
            continue;
        }
        else if (new_user == 'n')
        {
            string existing_user_id;
            string existing_user_password;
            string existing_user_name;
            string existing_user_type;
            int existing_user_number_of_book_issued;
            vector<Book> existing_user_ListOfBookIssued;

            cout << "Welcome to the Login page." << endl;
            cout << flush;

            cout << endl
                 << "enter your id : " << endl;
            cout << flush;
            cin >> existing_user_id;

            cout << "enter your Password : " << endl;
            cout << flush;
            cin >> existing_user_password;

            if (userDatabase.validId(existing_user_id, &userDatabase) == 1)
            {
                if (userDatabase.validPassword(existing_user_id, existing_user_password, &userDatabase) == 1)
                {
                    cout << "You have successfully login" << endl;
                    cout << flush;

                    existing_user_name = userDatabase.Search(existing_user_id, &userDatabase).Name;
                    existing_user_type = userDatabase.Search(existing_user_id, &userDatabase).UserType;

                    while (1)
                    {
                        if (existing_user_type == "student")
                        {
                            existing_user_number_of_book_issued = userDatabase.Search(existing_user_id, &userDatabase).number_of_book_issued;
                            existing_user_ListOfBookIssued = userDatabase.Search(existing_user_id, &userDatabase).ListOfBookIssued;
                            cout << "select 1 for showing the list of books issued by you \nselect 2 for showing the list of all the available books" << endl;
                            cout << flush;
                            cout << "select 3 for updating your password \nselect 4 for returning the book and calculating your fine amount \nselect 5 for issuing a book" << endl;
                            cout << flush;
                            cout << "select 0 to logout." << endl;
                            cout << flush;
                            int query_response;
                            cin >> query_response;
                            if (query_response == 1)
                            {
                                cout << "you have " << existing_user_ListOfBookIssued.size() << " books issued" << endl;
                                cout << flush;
                                for (int i = 0; i < existing_user_ListOfBookIssued.size(); i++)
                                {
                                    cout << "Title of the book : " << existing_user_ListOfBookIssued[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << existing_user_ListOfBookIssued[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << existing_user_ListOfBookIssued[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << existing_user_ListOfBookIssued[i].Publication << endl;
                                    cout << flush;
                                    time_t due_time = existing_user_ListOfBookIssued[i].DueTime;
                                    time_t last_issued_time = existing_user_ListOfBookIssued[i].LastIssueTime;
                                    tm *ltm = localtime(&due_time);
                                    tm *ltm1 = localtime(&last_issued_time);

                                    cout << "Date of the issue : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
                                    cout << flush;

                                    cout << "Due Date of the book : " << ltm1->tm_mday << " " << 1 + ltm1->tm_mon << " " << 1900 + ltm1->tm_year << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            }

                            if (query_response == 2)
                            {
                                cout << "There are " << bookDatabase.ListOfBooks.size() << " books in database" << endl;
                                cout << flush;
                                for (int i = 0; i < bookDatabase.ListOfBooks.size(); i++)
                                {
                                    cout << "Title of the book : " << bookDatabase.ListOfBooks[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << bookDatabase.ListOfBooks[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << bookDatabase.ListOfBooks[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << bookDatabase.ListOfBooks[i].Publication << endl;
                                    cout << flush;
                                    cout << "Availability status of the book : " << bookDatabase.ListOfBooks[i].Availability << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            };

                            if (query_response == 3)
                            {
                                string new_pass;
                                cout << "Give new password : " << endl;
                                cout << flush;
                                cin >> new_pass;
                                userDatabase.Update(existing_user_id, existing_user_name, new_pass, existing_user_type, &userDatabase);
                            }

                            if (query_response == 4)
                            {
                                string book_return_isbn;
                                cout << "Give the ISBN of the book you want to issue : " << endl;
                                cout << flush;
                                cin >> book_return_isbn;
                                userDatabase.ReturnBook(book_return_isbn, existing_user_id, &bookDatabase, &userDatabase);
                            };

                            if (query_response == 5)
                            {
                                string book_issue_isbn;
                                cout << "Give the ISBN of the book you want to issue : " << endl;
                                cout << flush;
                                cin >> book_issue_isbn;
                                userDatabase.BookRequestStudent(book_issue_isbn, existing_user_id, &bookDatabase, &userDatabase);
                            };

                            if (query_response == 0)
                            {
                                break;
                            }
                        }

                        if (existing_user_type == "professor")
                        {
                            existing_user_number_of_book_issued = userDatabase.Search(existing_user_id, &userDatabase).number_of_book_issued;
                            existing_user_ListOfBookIssued = userDatabase.Search(existing_user_id, &userDatabase).ListOfBookIssued;
                            cout << "select 1 for showing the list of books issued by you \nselect 2 for showing the list of all the available books" << endl;
                            cout << flush;
                            cout << "select 3 for updating your password \nselect 4 for returning the book and calculating your fine amount \nselect 5 for issuing a book" << endl;
                            cout << flush;
                            cout << "select 0 to logout." << endl;
                            cout << flush;
                            int query_response;
                            cin >> query_response;
                            if (query_response == 1)
                            {
                                cout << "you have " << existing_user_ListOfBookIssued.size() << " books issued" << endl;
                                cout << flush;
                                for (int i = 0; i < existing_user_ListOfBookIssued.size(); i++)
                                {
                                    cout << "Title of the book : " << existing_user_ListOfBookIssued[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << existing_user_ListOfBookIssued[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << existing_user_ListOfBookIssued[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << existing_user_ListOfBookIssued[i].Publication << endl;
                                    cout << flush;

                                    time_t due_time = existing_user_ListOfBookIssued[i].DueTime;
                                    time_t last_issued_time = existing_user_ListOfBookIssued[i].LastIssueTime;
                                    tm *ltm = localtime(&due_time);
                                    tm *ltm1 = localtime(&last_issued_time);

                                    cout << "Date of the issue : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
                                    cout << flush;

                                    cout << "Due Date of the book : " << ltm1->tm_mday << " " << 1 + ltm1->tm_mon << " " << 1900 + ltm1->tm_year << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            }

                            if (query_response == 2)
                            {
                                cout << "There are " << bookDatabase.ListOfBooks.size() << " books in database" << endl;
                                cout << flush;
                                for (int i = 0; i < bookDatabase.ListOfBooks.size(); i++)
                                {
                                    cout << "Title of the book : " << bookDatabase.ListOfBooks[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << bookDatabase.ListOfBooks[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << bookDatabase.ListOfBooks[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << bookDatabase.ListOfBooks[i].Publication << endl;
                                    cout << flush;
                                    cout << "Availability status of the book : " << bookDatabase.ListOfBooks[i].Availability << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            };

                            if (query_response == 3)
                            {
                                string new_pass;
                                cout << "Give new password : " << endl;
                                cout << flush;
                                cin >> new_pass;
                                userDatabase.Update(existing_user_id, existing_user_name, new_pass, existing_user_type, &userDatabase);
                            }

                            if (query_response == 4)
                            {
                                string book_return_isbn;
                                cout << "Give the ISBN of the book you want to issue : " << endl;
                                cout << flush;
                                cin >> book_return_isbn;
                                userDatabase.ReturnBook(book_return_isbn, existing_user_id, &bookDatabase, &userDatabase);
                            };

                            if (query_response == 5)
                            {
                                string book_issue_isbn;
                                cout << "Give the ISBN of the book you want to issue : " << endl;
                                cout << flush;
                                cin >> book_issue_isbn;
                                userDatabase.BookRequestProf(book_issue_isbn, existing_user_id, &bookDatabase, &userDatabase);
                            };

                            if (query_response == 0)
                            {
                                break;
                            };
                        }

                        if (existing_user_type == "librarian")
                        {
                            cout << "select 1 for showing the list of books issued by any user \nselect 2 for showing the list of all the available books" << endl;
                            cout << flush;
                            cout << "select 3 for adding new book \nselect 4 for adding new user \nselect 5 for updating book details " << endl;
                            cout << flush;
                            cout << "select 6 for updating user detail \nselect 7 for deleting a books \nselect 8 for deleting a user" << endl;
                            cout << flush;
                            cout << "select 0 to logout." << endl;
                            cout << flush;

                            int query_response;
                            cin >> query_response;
                            if (query_response == 1)
                            {
                                string id;
                                int selected_user_number_of_book_issued;
                                vector<Book> selected_user_ListOfBookIssued;
                                cout << "Enter the id of the user whose issue list you want to check : " << endl;
                                cout << flush;
                                cin >> id;
                                selected_user_number_of_book_issued = userDatabase.Search(id, &userDatabase).number_of_book_issued;
                                selected_user_ListOfBookIssued = userDatabase.Search(id, &userDatabase).ListOfBookIssued;
                                cout << "This user has " << selected_user_ListOfBookIssued.size() << " books issued" << endl;
                                cout << flush;
                                for (int i = 0; i < selected_user_ListOfBookIssued.size(); i++)
                                {
                                    cout << "Title of the book : " << selected_user_ListOfBookIssued[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << selected_user_ListOfBookIssued[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << selected_user_ListOfBookIssued[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << selected_user_ListOfBookIssued[i].Publication << endl;
                                    cout << flush;

                                    time_t due_time = existing_user_ListOfBookIssued[i].DueTime;
                                    time_t last_issued_time = existing_user_ListOfBookIssued[i].LastIssueTime;
                                    tm *ltm = localtime(&due_time);
                                    tm *ltm1 = localtime(&last_issued_time);

                                    cout << "Date of the issue : " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 + ltm->tm_year << endl;
                                    cout << flush;

                                    cout << "Due Date of the book : " << ltm1->tm_mday << " " << 1 + ltm1->tm_mon << " " << 1900 + ltm1->tm_year << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            }

                            if (query_response == 2)
                            {
                                cout << "There are " << bookDatabase.ListOfBooks.size() << " books in database" << endl;
                                cout << flush;
                                for (int i = 0; i < bookDatabase.ListOfBooks.size(); i++)
                                {
                                    cout << "Title of the book : " << bookDatabase.ListOfBooks[i].Title << endl;
                                    cout << flush;
                                    cout << "Author of the book : " << bookDatabase.ListOfBooks[i].Author << endl;
                                    cout << flush;
                                    cout << "ISBN of the book : " << bookDatabase.ListOfBooks[i].ISBN << endl;
                                    cout << flush;
                                    cout << "Publication of the book : " << bookDatabase.ListOfBooks[i].Publication << endl;
                                    cout << flush;
                                    cout << "Availability status of the book : " << bookDatabase.ListOfBooks[i].Availability << endl;
                                    cout << flush;
                                }
                                cout << endl;
                                cout << flush;
                            };

                            if (query_response == 3)
                            {
                                string new_book_title;
                                string new_book_author;
                                string new_book_isbn;
                                string new_book_publication;
                                cout << "Title of the book : " << endl;
                                cout << flush;
                                cin >> new_book_title;
                                cout << "Author of the book : " << endl;
                                cout << flush;
                                cin >> new_book_author;
                                cout << "ISBN of the book : " << endl;
                                cout << flush;
                                cin >> new_book_isbn;
                                cout << "Publication of the book : " << endl;
                                cout << flush;
                                cin >> new_book_publication;
                                bookDatabase.Add(new_book_title, new_book_author, new_book_isbn, new_book_publication, "is available", &bookDatabase);

                                cout << "Book successfully added to the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 4)
                            {
                                string new_user_name;
                                string new_user_id;
                                string new_user_password = new_user_name;
                                string new_user_type;
                                cout << "Name of the user : " << endl;
                                cout << flush;
                                cin >> new_user_name;
                                cout << "ID of the user : " << endl;
                                cout << flush;
                                cin >> new_user_id;
                                cout << "Type of the user : " << endl;
                                cout << flush;
                                cin >> new_user_type;
                                userDatabase.Add(new_user_name, new_user_id, new_user_password, new_user_type, &userDatabase);

                                cout << "User successfully added to the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 5)
                            {
                                string new_title;
                                string new_author;
                                string isbn;
                                string new_publication;
                                cout << "Title of the book : " << endl;
                                cout << flush;
                                cin >> new_title;
                                cout << "Author of the book : " << endl;
                                cout << flush;
                                cin >> new_author;
                                cout << "Publication of the book : " << endl;
                                cout << flush;
                                cin >> new_publication;
                                cout << "ISBN of the book to be updated : " << endl;
                                cout << flush;
                                cin >> new_publication;
                                bookDatabase.Update(new_title, new_author, isbn, new_publication, "is available", &bookDatabase);

                                cout << "Book successfully added to the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 6)
                            {
                                string new_name;
                                string new_password = "master";
                                string id;
                                string new_userType;
                                cout << "name of the user : " << endl;
                                cout << flush;
                                cin >> new_name;
                                cout << "User Type : " << endl;
                                cout << flush;
                                cin >> new_userType;
                                cout << "ID of the user to be updated : " << endl;
                                cout << flush;
                                cin >> id;
                                userDatabase.Update(id, new_name, new_password, new_userType, &userDatabase);

                                cout << "Book successfully updated in the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 7)
                            {
                                string isbn;
                                cout << "ID of the book to be deleted : " << endl;
                                cout << flush;
                                cin >> isbn;
                                bookDatabase.Delete(isbn, &bookDatabase);

                                cout << "Book successfully deleted from the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 8)
                            {
                                string id;
                                cout << "ID of the user to be deleted : " << endl;
                                cout << flush;
                                cin >> id;
                                userDatabase.Delete(id, &userDatabase);

                                cout << "User successfully deleted from the database." << endl;
                                cout << flush;
                            }

                            if (query_response == 0)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    cout << "Please enter correct password" << endl;
                    cout << flush;
                }
            }
            else
            {
                cout << "Please enter valid ID" << endl;
                cout << flush;
            }
        }
        else if (new_user = 'e')
        {
            break;
        }
        else { 
            cout<<"fgfg";
            cout << "Select either y or n" << endl;
            cout << flush;
            continue;
        }
    }
    return 0;
}
