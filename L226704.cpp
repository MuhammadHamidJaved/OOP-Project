#include<iostream>
#include<fstream>
using namespace std;

class facebook;
class User;
class Page;
class Activity;
class Date;
class Object;
class Memory;
class Comment;
class Post;

class Helper
{
public:
    static int StringLenght(const char* str)
    {

        int count = 0;
        for (int i = 0; str[i] != '\0'; i++)
        {
            count++;
        }
        return count;

    }
    static void StringCopy(char* dest, const char* src)
    {

        int i;
        for (i = 0; src[i] != '\0'; i++)
        {
            dest[i] = src[i];
        }
        dest[i] = '\0';

    }
    static char* GetStringFromBuffer(const char* str)
    {

        int size = StringLenght(str);
        char* string = new char[size + 1];
        StringCopy(string, str);
        return string;

    }
    static bool searchString(char* myStr,const char* subString)
    {
        int size = StringLenght(subString);
        for (int i = 0; myStr[i] != '\0'; i++)
        {
            if (myStr[i] == subString[0])
            {
                int c = i;
                int j;
                for (j = 0; subString[j] != '\0'; j++)
                {
                    if (myStr[c++] != subString[j])
                    {

                        break;
                    }
                }
                if (j == size)
                    return true;

            }
        }

        return false;

    }
};

class Object      //Parent class
{
private:
    char* ID;
public:
    Object()
    {
        ID = 0;
    }
    char* GetId()     //getter for id in polymorph classes
    {
        return ID;
    }
    void SetID(char* _Id)    //Setter of ID
    {
        ID = _Id;
    }

    virtual void AddPostToTimeline(Post* post)
    {
        cout << "AddPostToTimeline() called";
    }

    virtual void PrintObject()
    {
        cout << "Print() called";
    }

    virtual void PrintLikePost()
    {
        cout << "Print() called";
    }

    ~Object()
    {
        delete[] ID;
        ID = 0;
    }

};

class Activity
{
private:
    int Type;
    char* value;
public:
    Activity()
    {
        Type = 0;
        value = 0;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        fin >> Type;
        char temp[100];
        fin.getline(temp, 100);
        value = Helper::GetStringFromBuffer(temp);
    }
    void PrintActivity()
    {
        if (Type == 1)
        {
            cout << " is feeling ";
            cout << value;
        }
        else if (Type == 2)
        {
            cout << " is thinking about ";
            cout << value;
        }
        else if (Type == 3)
        {
            cout << " is Making ";
            cout << value;
        }
        else if (Type == 4)
        {
            cout << " is celebrating ";
            cout << value;
        }

    }
    ~Activity()
    {
        if (value != 0)
        {
            delete[] value;
            value = 0;
        }
    }
};

class Date
{
private:
    int day;
    int month;
    int year;
public:
    static Date CurrentDate;
    Date(int _day = 0, int _month = 0, int _year = 0)
    {
        day = _day;
        month = _month;
        year = _year;
    }
    void SetCurrentDate(int _day, int _month, int _year)
    {
        CurrentDate.day = _day;
        CurrentDate.month = _month;
        CurrentDate.year = _year;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        fin >> day;
        fin >> month;
        fin >> year;
    }
    void PrintDate()
    {
        cout << day << "/" << month << "/" << year << endl;
    }

    bool CompareDate(Date rhs)
    {
        if (day == rhs.day && month == rhs.month && year == rhs.year)
        {
            return true;
        }
        else if (day == rhs.day - 1 && month == rhs.month && year == rhs.year)
        {
            return true;
        }
        else if (day == rhs.day + 1 && month == rhs.month && year == rhs.year)
        {
            return true;
        }
        else
            return false;
    }
    int YearsOfMemory(Date rhs)
    {
        return year - rhs.year;
    }

};

Date Date::CurrentDate(0,0,0);

class Comment
{
private:
    char* ID;
    char* text;
    Object* commentBy;
public:
    Comment()
    {
        ID = 0;
        text = 0;
        commentBy = 0;
    }
    void SetValues(char* id,const char* _text, Object* _commentBy)
    {
        ID = Helper::GetStringFromBuffer(id);
        text = Helper::GetStringFromBuffer(_text);
        commentBy = _commentBy;
    }
    ~Comment()
    {
        if (ID != 0)
        {
            delete[] ID;
            ID = 0;
        }
        if (text != 0)
        {
            delete[] text;
            text = 0;
        }
        commentBy = 0;
    }
    void PrintComment()
    {
        cout << "\t\t\t";
        commentBy->PrintObject();
        cout << " wrote:" << text;
        cout << endl;
    }
    
};

class Post {
private:
    char* text;
    char* id;
    Date SharedDate;
    Activity* activity;
    Object* SharedBy;
    Object** LikedBy;
    int totalComments, totalLikes;
    static int totalPosts;
    Comment** comments;
    int postType;
public:
    Post()
    {
        text = 0;
        id = 0;
        activity = 0;
        SharedBy = 0;
        LikedBy = 0;
        comments = 0;
        totalComments = 0;
        totalLikes = 0;
        postType = 0;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        char temp[80];
        fin >> postType;
        fin.ignore();
        fin >> temp;
        id = Helper::GetStringFromBuffer(temp);
        SharedDate.ReadDataFromFile(fin);
        fin.ignore();
        fin.getline(temp, 80, '\n');
        text = Helper::GetStringFromBuffer(temp);
        if (postType == 2)
        {
            activity = new Activity;
            activity->ReadDataFromFile(fin);
        }
    }
    char* GetId()
    {
        return id;
    }
    void SetSharedBy(Object* objectPtr)
    {
        SharedBy = objectPtr;
    }
    void SetLikedBy(Object* objectPtr)
    {
        if (totalLikes == 0)
        {
            LikedBy = new Object * [10];
        }
        LikedBy[totalLikes] = objectPtr;
        totalLikes++;
    }
    static void SetTotalPosts(int totalPost)
    {
        totalPosts = totalPost;
    }
    static int GetTotalPosts()
    {
        return totalPosts;
    }
    void AddComment(Comment* comment)
    {
        if (totalComments == 0)
        {
            comments = new Comment * [10];
        }
        comments[totalComments] = comment;
        totalComments++;
    }

    void ShowLikedList()
    {
        for (int i = 0; i < totalLikes; i++)
        {
            LikedBy[i]->PrintLikePost();
        }
    }

    void PrintPost()
    {
        if (postType == 2)
        {
            cout << "---";
            SharedBy->PrintObject();
            activity->PrintActivity();
            cout << endl;
        }
        else
        {
            cout << "---";
            SharedBy->PrintObject();
            cout << " shared ";
        }
        cout << "'" << text << "'";
        SharedDate.PrintDate();
        for (int i = 0; i < totalComments; i++)
        {
            comments[i]->PrintComment();
        }
        cout << "\n\n";
    }

    Object* GetSharedBy()
    {
        return SharedBy;
    }
    Date GetSharedDate()
    {
        return SharedDate;
    }
    bool CompareDatesOfPosts()
    {
        if (Date::CurrentDate.CompareDate(SharedDate) == true)
        {
            return true;
        }
        else
            return false;
    }
    ~Post()
    {
        if (text != 0)
        {
            delete[] text;
            text = 0;
        }
        if (id != 0)
        {
            delete[] id;
            id = 0;
        }
        activity = 0;
        SharedBy = 0;
        for (int i = 0; i < totalLikes; i++)
        {
            LikedBy = 0;
        }
        delete[] LikedBy;
        for (int i = 0; i < totalComments; i++)
        {
            comments = 0;
        }
        delete[] comments;
    }

};

class Memory : public Post
{
private:
    Post* originalPost;
    char* text;
public:
    Memory()
    {
        originalPost=0;
        text=0;
    }
    void SetMemory(Post* post,const char* _text)
    {
        originalPost = post;
        text = Helper::GetStringFromBuffer(_text);
    }
    void PrintMemory()
    {
        cout << text;
        cout << "\n\t\t\t(" << Date::CurrentDate.YearsOfMemory(originalPost->GetSharedDate()) << " years ago)\n";
        cout << endl;
        originalPost->PrintPost();
    }
    ~Memory()
    {
        originalPost = 0;
        if (text != 0)
        {
            delete[] text;
            text = 0;
        }
    }
};

class Page :public Object    //polymorphism
{
private:
    char* Title;
    static int totalPages;
    int totalPosts = 0;
    Post** timeline;
    Memory* memory;
public:
    Page()
    {
        Title = 0;
        timeline = 0;
        memory = 0;
    }
    static void SetTotalPages(int pages)
    {
        totalPages = pages;
    }

    static int GetTotalPages()
    {
        return totalPages;
    }

    char* GetTitle()
    {
        return Title;
    }

    void ReadDataFromFile(ifstream& fin)    //function of page  class to read pages from file
    {
        char temp[50];

        fin >> temp;
        SetID(Helper::GetStringFromBuffer(temp));
        fin.getline(temp, 50, '\n');
        Title = Helper::GetStringFromBuffer(temp);
    }

    void printPages()            //function to print pages
    {
        cout << GetId() << "\t" << Title << endl;
    }

    void PrintLikePost()            //function to print liked posts
    {
        cout << GetId() << "-" << Title << endl;
    }

    Post** GetPost()
    {
        return timeline;
    }

    ~Page()
    {
        if (Title != 0)
        {
            delete[] Title;
            Title = 0;
        }
        for (int i = 0; i < totalPosts; i++)
        {
            timeline[i] = 0;
        }
        delete[] timeline;
        memory = 0;

    }

    void ShareMemory(char* postId, char* text)
    {
        memory = new Memory;
        for (int i = 0; i < totalPosts; i++)
        {
            if (Helper::searchString(timeline[i]->GetId(), postId) == true)
            {
                memory->SetMemory(timeline[i], text);
                break;
            }
        }
    }

    void AddPostToTimeline(Post* post)    //function to add posts to timeline
    {
        if (totalPosts == 0)
        {
            timeline = new Post * [10];
        }
        timeline[totalPosts] = post;
        totalPosts++;
    }
    void PrintObject()
    {
        cout << Title << " ";
    }

    void ViewTimeline()
    {
        if (memory != 0)
        {
            memory->GetSharedBy()->PrintObject();
            cout << " Shared a memory~~~";
            Date::CurrentDate.PrintDate();
            memory->PrintMemory();
        }
        for (int i = 0; i < totalPosts; i++)
        {
            timeline[i]->PrintPost();
        }

    }

    void PrintLatestPosts()
    {
        for (int j = 0; j < totalPosts; j++)
        {
            if (timeline[j]->CompareDatesOfPosts() == true)
            {
                timeline[j]->PrintPost();

            }
        }
    }

};

class User :public Object   //polymorphism
{
private:
    char* firstName;
    char* lastName;
    Page** likedPages;
    static int totalUsers;
    int totalLikedPages;
    int totalFriends;
    User** friendList;
    Post** timeline;
    int totalPosts;
    Memory* memory;
public:

    User()
    {
        firstName = 0;
        lastName = 0;
        likedPages = 0;
        friendList = 0;
        totalLikedPages = 0;
        totalFriends = 0;
        timeline = 0;
        totalPosts = 0;
        memory = 0;
    }
    char* GetFirstName()    //getter for name
    {
        return firstName;
    }
    char* GetLastName()
    {
        return lastName;
    }

    static void SetTotalUsers(int users)
    {
        totalUsers = users;
    }

    static int GetTotalUsers()
    {
        return totalUsers;
    }

    void ReadDataFromFile(ifstream& fin)    //function of user class to read users from file
    {
        char temp[30];
        fin >> temp;
        SetID(Helper::GetStringFromBuffer(temp));
        fin >> temp;
        firstName = Helper::GetStringFromBuffer(temp);
        fin >> temp;
        lastName = Helper::GetStringFromBuffer(temp);

    }

    void PrintUser()      //function to print users
    {
        cout << GetId() << "  " << firstName << "  " << lastName << endl;
    }

    void LikePage(Page* pagePtr)    //function for association of liked pages
    {
        if (totalLikedPages == 0)
        {
            likedPages = new Page * [10];
        }
        likedPages[totalLikedPages] = pagePtr;
        totalLikedPages++;
    }
 
    void AddFriend(User* userPtr)    //function for association of friends
    {
        if (totalFriends == 0)
        {
            friendList = new User * [10];
        }
        friendList[totalFriends] = userPtr;
        totalFriends++;
    }

    void PrintLikePost()            //function to print liked posts
    {
        cout << GetId() << "- " << firstName << " " << lastName << endl;
    }

    void PrintObject()
    {
        cout << firstName << " " << lastName;
    }
    void ViewFriendList()     //function to show friend list
    {
        cout << firstName << " " << lastName << " - Friend List\n\n";
        if (totalFriends == 0)
        {
            cout << firstName << " " << lastName << " has NO friends.\n";
        }
        else
        {
            for (int i = 0; i < totalFriends; i++)
            {
                friendList[i][0].PrintUser();
            }
        }

    }
    void ViewLikedPages()    //function to show liked pages
    {
        cout << firstName << " " << lastName << " - Liked Pages\n\n";
        if (totalLikedPages == 0)
        {
            cout << firstName << " " << lastName << " has NOT liked any Page.\n";
        }
        else
        {
            for (int i = 0; i < totalLikedPages; i++)
            {
                likedPages[i][0].printPages();
            }
        }
    }

    void AddPostToTimeline(Post* post)    //function to add posts to timeline
    {
        if (totalPosts == 0)
        {
            timeline = new Post * [10];
        }
        timeline[totalPosts] = post;
        totalPosts++;
    }

    void PrintPostsOfHome()
    {
        for (int i = 0; i < totalFriends; i++)
        {
            for (int j = 0; j < friendList[i]->totalPosts; j++)
            {
                if (friendList[i]->timeline[j]->CompareDatesOfPosts() == true)
                {
                    friendList[i]->timeline[j]->PrintPost();

                }
            }
        }
    }

    void PrintLatestPosts()
    {
        for (int j = 0; j < totalPosts; j++)
        {
            if (timeline[j]->CompareDatesOfPosts() == true)
            {
                timeline[j]->PrintPost();

            }
        }
    }

    void ViewHome()
    {
        for (int i = 0; i < totalFriends; i++)
        {
            friendList[i]->PrintLatestPosts();
        }
        for (int i = 0; i < totalLikedPages; i++)
        {   
            likedPages[i]->PrintLatestPosts();
        }
    }

    void LikePost(Post* postPtr)
    {
        postPtr->SetLikedBy(this);
    }

    void PostComment(Post* postPtr, const char* text)
    {
        char tempId[] = "0";
        Comment* comment = new Comment;
        postPtr->AddComment(comment);
        comment->SetValues(tempId, text, this);
    }

    ~User()
    {
        if (firstName != 0)
        {
            delete[] firstName;
            firstName = 0;
        }
        if (lastName != 0)
        {
            delete[] lastName;
            lastName = 0;
        }
        for (int i = 0; i < totalLikedPages; i++)
        {
            likedPages[i] = 0;
        }
        delete[] likedPages;
        for (int i = 0; i < totalFriends; i++)
        {
            friendList[i] = 0;
        }
        delete[] friendList;
        for (int i = 0; i < totalPosts; i++)
        {
            timeline[i] = 0;
        }
        delete[] timeline;
        memory = 0;
    }
  
    void ShareMemory(const char* postId, const char* text)
    {
        memory = new Memory;
        for (int i = 0; i < totalPosts; i++)
        {
            if (Helper::searchString(timeline[i]->GetId(), postId) == true)
            {
                memory->SetMemory(timeline[i], text);
                break;
            }
        }
    }

    void SeeYourMemories()
    {
        
        for (int i = 0; i < totalPosts; i++)
        {
            if (Date::CurrentDate.YearsOfMemory(timeline[i]->GetSharedDate()) >= 1)
            {
                cout << "On this Day\n";
                cout << "(" << Date::CurrentDate.YearsOfMemory(timeline[i]->GetSharedDate()) << " years ago)\n";
                timeline[i]->PrintPost();
            }  
        }
    }

    void ViewTimeline()
    {
        if (memory != 0)
        {
            cout << firstName << " " << lastName;
            cout << " Shared a memory~~~";
            Date::CurrentDate.PrintDate();
            memory->PrintMemory();
        }
        for (int i = 0; i < totalPosts; i++)
        {
            timeline[i]->PrintPost();
        }

    }

};

int User::totalUsers = 0;
int Page::totalPages = 0;
int Post::totalPosts = 0;

class facebook    //Controller Class
{
private:
    Page** pages;
    User** users;
    Post** posts;
public:
    facebook()           //constructor for controller class
    {
        pages = 0;
        users = 0;
        posts = 0;
    }

    void LoadData()         //Data from file
    {
        char temp[] = "SocialNetworkPages.txt";
        pages = LoadPagesFromFile(temp);
        char temp1[] = "SocialNetworkUsers.txt";
        users = LoadUsersFromFile(temp1);
        char temp2[] = "SocialNetworkPosts.txt";
        posts = LoadPostsFromFile(temp2);
        char temp3[] = "SocialNetworkComments.txt";
        LoadCommentsFromFile(temp3);
    }

    void Run()
    {
       
        Date::CurrentDate.SetCurrentDate(15, 11, 2017);
        User* currentuser = SearchUserByID("u7");
        cout << "Command:\tSet current System Date 15 11 2017\n";
        cout << "System Date:\t";
        Date::CurrentDate.PrintDate();
        cout << "\nCommand:\t Set current user " << currentuser[0].GetId() << "\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " successfully set as Current User\n\n\n";
        cout << "Command:\t View Friend List \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        currentuser->ViewFriendList();
        cout << "-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t View Liked Pages \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        currentuser->ViewLikedPages();
        cout << "-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\tViewHome \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " - HomePage\n\n";
        currentuser->ViewHome();
        cout << "-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t View Timeline \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " -Timeline\n\n";
        currentuser->ViewTimeline();
        cout << "-----------------------------------------------------------------------------------------\n\n";
        Post* postPtr = SearchPostByID("post5");
        cout << "Command:\t ViewLikedList(post5) \n\n";
        cout << "Post liked By\n";
        ViewLikedList("post5");
        cout << "\n-----------------------------------------------------------------------------------------\n\n";

        cout << "Command:\t LikePost(post5) \n";
        currentuser->LikePost(postPtr);
        cout << "Command:\t ViewLikedList(post5) \n\n";
        cout << "Post liked By\n";
        ViewLikedList("post5");
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t   PostComment(post4, Good Luck for your Result) \n";
        postPtr = SearchPostByID("post4");
        currentuser->PostComment(postPtr, "Good Luck for your Result");
        cout << "Command:\t   ViewPost(post4) \n";
        ViewPost(postPtr);
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t   PostComment(post8, Thanks for the wishes) \n";
        postPtr = SearchPostByID("post8");
        currentuser->PostComment(postPtr,"Thanks for the wishes");
        cout << "Command:\t   ViewPost(post8) \n";
        ViewPost(postPtr);
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\tShareMemory(post10, Never thought I will be specialist in this field...) \n";
        currentuser->ShareMemory("post10", "Never thought I will be specialist in this field...");
        cout << "Command:\t   Timeline \n";
        currentuser->ViewTimeline();
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t   SeeYourMemories \n";
        currentuser->SeeYourMemories();
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\tViewPage(p1)\n\n";
        Page* currentPage = SearchPageByID("p1");
        cout << currentPage->GetTitle()<<endl;
        currentPage->ViewTimeline();
        cout << "\n-----------------------------------------------------------------------------------------\n\n";
        currentuser = SearchUserByID("u11");
        cout << "\nCommand:\t Set current user " << currentuser[0].GetId() << "\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " successfully set as Current User\n\n\n";
        cout << "Command:\tViewHome \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " - HomePage \n\n";
        currentuser->ViewHome();
        cout << "-----------------------------------------------------------------------------------------\n\n";
        cout << "Command:\t View Timeline \n";
        cout << "-----------------------------------------------------------------------------------------\n";
        cout << currentuser[0].GetFirstName() << " " << currentuser[0].GetLastName() << " -Timeline\n\n";
        currentuser->ViewTimeline();
        cout << "-----------------------------------------------------------------------------------------\n\n";
    }
    
    Page** LoadPagesFromFile(char* fileName)     //function of controller class to read pages from file
    {
        ifstream fin;

        int totalPage;
        char buffer[80];
        fin.open(fileName);
        if (fin.is_open())
        {
            fin >> totalPage;
            Page::SetTotalPages(totalPage);
            fin.getline(buffer, 80, '\n');
            pages = new Page * [totalPage];
            for (int i = 0; i < totalPage; i++)
            {
                pages[i] = new Page;
                pages[i][0].ReadDataFromFile(fin);
            }

            return pages;
        }
        else
            return 0;

    }
 
    User** LoadUsersFromFile(char* fileName)     //function of controller class to read users from file
    {
        ifstream fin;
        int totalUsers;
        char buffer[100];
        fin.open(fileName);
        if (fin.is_open())
        {
            fin >> totalUsers;
            User::SetTotalUsers(totalUsers);
            fin.getline(buffer, 100, '\n');
            users = new User * [totalUsers];
            char tempfriendID[10];
            char pageID[10];
            char*** tempFriendList = new char** [totalUsers];     //triple pointer to temporarily store the users.It points to double pointers 
            //of total users which further points to max of 10 users as friends cannot be more than 10.
            int j;
            for (int i = 0; i < totalUsers; i++)
            {
                users[i] = new User;
                users[i][0].ReadDataFromFile(fin);
                tempFriendList[i] = new char* [10];
                for (j = 0; j < 10; j++)
                {
                    fin >> tempfriendID;
                    if (tempfriendID[0] == '-')  //break the loop if it gets -1 from file 
                    {
                        break;
                    }
                    int length;
                    {
                        length = Helper::StringLenght(tempfriendID);
                        tempFriendList[i][j] = new char[length + 1];
                        for (int k = 0; k < length; k++)
                        {
                            tempFriendList[i][j][k] = tempfriendID[k];
                        }
                        tempFriendList[i][j][length] = '\0';
                    }

                }
                for (j; j < 10; j++)       //Loop for searching and association of pages
                {
                    tempFriendList[i][j] = 0;
                }
                fin >> pageID;
                while (pageID[0] != '-')
                {
                    Page* pagePtr = SearchPageByID(pageID);
                    users[i]->LikePage(pagePtr);
                    fin >> pageID;
                }

            }
            for (int i = 0; i < totalUsers; i++)  //Loop for searching and association of users
            {
                for (int k = 0; k < 10; k++)
                {
                    if (tempFriendList[i][k] != 0)
                    {
                        User* userPtr = SearchUserByID(tempFriendList[i][k]);
                        users[i]->AddFriend(userPtr);
                    }
                }
            }
            for (int i = 0; i < totalUsers; i++)  //Deallocation of triple pointer
            {
                for (int j = 0; j < 10; j++)
                {
                    delete[] tempFriendList[i][j];
                }
            }
            delete[] tempFriendList;
            tempFriendList = 0;    //Making triple pointer null
            return users;
        }
        else
            return 0;


    }

    Post** LoadPostsFromFile(char* fileName)
    {
        ifstream fin;
        int totalPosts;
        char sharedBy[10];
        char likedBy[10];
        fin.open(fileName);
        if (fin.is_open())
        {
            fin >> totalPosts;
            fin.ignore();
            Post::SetTotalPosts(totalPosts);
            posts = new Post * [totalPosts];
            for (int i = 0; i < totalPosts; i++)
            {
                posts[i] = new Post;
                posts[i][0].ReadDataFromFile(fin);
                fin >> sharedBy;
                Object* objectPtr = SearchObjectByID(sharedBy);
                posts[i]->SetSharedBy(objectPtr);
                objectPtr->AddPostToTimeline(posts[i]);
                fin >> likedBy;
                while (likedBy[0] != '-')
                {
                    Object* likedByPtr = SearchObjectByID(likedBy);
                    posts[i]->SetLikedBy(likedByPtr);
                    fin >> likedBy;
                }
            }
            return posts;
        }
        else
            return 0;
    }
    
    void LoadCommentsFromFile(char* fileName)
    {
        ifstream fin;
        int totalComments;
        char Buffer[100];
        char CommentBy[10];
        fin.open(fileName);
        if (fin.is_open())
        {
            fin >> totalComments;
            for (int i = 0; i < totalComments; i++)
            {
                char* text;
                char id[10];
                char postID[10];
                Comment* comment = new Comment;
                fin >> id;
                fin >> postID;
                Post* postPtr = SearchPostByID(postID);
                postPtr->AddComment(comment);
                fin >> CommentBy;
                Object* commentBy = SearchObjectByID(CommentBy);
                fin.getline(Buffer, 100, '\n');
                text = Helper::GetStringFromBuffer(Buffer);
                comment->SetValues(id, text, commentBy);
            }
        }
    }
    
    Page* SearchPageByID(const char* pageID)    //function of controller class to search page by its ID
    {
        int totalPages = Page::GetTotalPages();
        for (int i = 0; i < totalPages; i++)
        {
            if (Helper::searchString(pages[i][0].GetId(), pageID) == true)
            {
                return pages[i];
            }
        }
        return 0;
    }

    User* SearchUserByID(const char* userID)     //function of controller class to search user by its ID
    {
        int totalUsers = User::GetTotalUsers();
        for (int i = 0; i < totalUsers; i++)
        {
            if (Helper::searchString(users[i][0].GetId(), userID) == true)
            {
                return users[i];
            }
        }
        return 0;
    }
   
    Object* SearchObjectByID(char* ObjectID)
    {
        if (ObjectID[0] == 'p')
        {
            int totalPages = Page::GetTotalPages();
            for (int i = 0; i < totalPages; i++)
            {
                if (Helper::searchString(pages[i][0].GetId(), ObjectID) == true)
                {
                    return pages[i];
                }
            }
            return 0;
        }
        else
        {
            int totalUsers = User::GetTotalUsers();
            for (int i = 0; i < totalUsers; i++)
            {
                if (Helper::searchString(users[i][0].GetId(), ObjectID) == true)
                {
                    return users[i];
                }
            }
            return 0;
        }
    }

    Post* SearchPostByID(const char* postID)    //function of controller class to search post by its ID
    {
        int totalPosts = Post::GetTotalPosts();
        for (int i = 0; i < totalPosts; i++)
        {
            if (Helper::searchString(posts[i][0].GetId(), postID) == true)
            {
                return posts[i];
            }
        }
        return 0;
    }

    void ViewLikedList(const char* postId)
    {
        Post* postPtr = SearchPostByID(postId);
        postPtr->ShowLikedList();
    }
    void ViewPost(Post* postPtr)
    {
        postPtr->PrintPost();
    }

    ~facebook()    //Destructor
    {
        int totalUsers = User::GetTotalUsers();
        int totalPages = Page::GetTotalPages();
        int totalPosts = Post::GetTotalPosts();
        if (users != 0)
        {
            for (int i = 0;i<totalUsers ; i++)
            {
                users[i] = 0;
            }
            delete[] users;
            users = 0;
        }
        if (pages != 0)
        {
            for (int i = 0; i < totalPages; i++)
            {
                pages[i] = 0;
            }
            delete[] pages;
            pages = 0;
        }
        if (posts != 0)
        {
            for (int i = 0; i < totalPosts; i++)
            {
                posts[i] = 0;
            }
            delete[] posts;
            posts = 0;
        }
    }
};

int main()
{
    facebook fb;
    fb.LoadData();
    fb.Run();

    return 0;
}
