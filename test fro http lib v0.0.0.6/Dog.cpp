#include "Dog.h"
#include "sqlite3.h"
#include <sstream>

#include <stdexcept>
constexpr auto DB_NAME = "mydb.sqlite";
#define createDB 0

http::json::JsonObject Dog::toJson() const noexcept
{
    http::json::JsonObject ret;
    ret.insert({ {"id"},{std::to_string(_id)} });
    ret.insert({ {"name"},{_name} });
    ret.insert({ {"bread"},{_bread} });
    ret.insert({ {"age"},{std::to_string(_age)} });
    std::string genderAsString;
    genderAsString += _gender;
    ret.insert({ { "gender" }, { genderAsString } });
    ret.insert({ {"path_to_picture"},{_pictureLocation} });
    return ret;
}

Dog Dog::New(int age, char gender, const std::string& name, const std::string& bread, const std::string& pathToPhoto)
{
    
    sqlite3* db = nullptr;
    int res = sqlite3_open(DB_NAME, &db);
    if (res)
    {
        std::cerr << "cant open db";
        exit(1);
    }
    

    res = sqlite3_exec(db, R"(
    create table if not exists Dogs(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        age int not null,
        gender text not null,
        breed text not null,
        name text not null,
        pathToPhoto text
        
    );

)", nullptr, nullptr, nullptr);

    if (res)
    {
        std::cerr << "err with creating dogs table:";
        std::cerr << sqlite3_errmsg(db);
        exit(1);
    }
    auto toSqlText = [](const std::string& str)->std::string {
        std::string ret = "\'";
        ret += str;
        ret += '\'';
        return ret;
    };
    std::stringstream insertStatment;
    insertStatment << "insert into dogs(age, gender, breed, name, pathToPhoto) " << "values ("
        << age <<','<< toSqlText(std::string(&gender))<<',' << toSqlText(bread)<<','<< toSqlText(name)<<',' << toSqlText(pathToPhoto);
    
    insertStatment << ");";
    std::cout << insertStatment.str() << '\n';
    res = sqlite3_exec(db, insertStatment.str().c_str(), nullptr, nullptr, nullptr);
    if (res)
    {
        std::cerr << "err with inserting dog table:";
        std::cerr << sqlite3_errmsg(db)<<'\n';
        throw std::runtime_error("cant insert dog into table");
    }

    int dogId = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);

    return Dog::GetById(dogId);
}

Dog Dog::GetById(int id)
{
    Dog ret;

    auto get_dog_callback = [](void* data, int argc, char** argv, char** column_names)->int {
        Dog* dog = static_cast<Dog*>(data);
        for (int i = 0; i < argc; i++) {
            std::string column_name(column_names[i]);
            if (column_name == "id") {
                dog->_id = std::stoi(argv[i]);
            }
            else if (column_name == "name") {
                dog->_name = std::string(argv[i]);
            }
            else if (column_name == "pathToPhoto") {
                dog->_pictureLocation = std::string(argv[i]);
            }
            else if (column_name == "breed") {
                dog->_bread = std::string(argv[i]);
            }
            else if (column_name == "age") {
                dog->_age = std::stoi(argv[i]);
            }
            else if (column_name == "gender") {
                dog->_gender = argv[i][0];
            }
        }
        return 0;
    };
    sqlite3* db = nullptr;
    int res = sqlite3_open(DB_NAME, &db);
    if (res)
    {
        std::cerr << "cant open db";
        exit(1);
    }

    std::string query = "select * from dogs where id=";
    query += std::to_string(id);
    query += ";";
    std::cout << query << '\n';
    res = sqlite3_exec(db, query.c_str(), get_dog_callback, &ret, nullptr);
    if (res)
    {
        std::cerr << "error while getting dog from db by id: " << id << " error: " << sqlite3_errmsg(db);
        throw std::runtime_error("error geeting dog");
    }
    sqlite3_close(db);
    return ret;
}

std::vector<Dog> Dog::getAllDogs()
{
   std::vector<Dog> ret;
   auto get_dog_callback = [](void* data, int argc, char** argv, char** column_names)->int {
       std::vector<Dog>* ret = static_cast<std::vector<Dog>*>(data);
       Dog dog;
       for (int i = 0; i < argc; i++) {
           std::string column_name(column_names[i]);
           if (column_name == "id") {
               dog._id = std::stoi(argv[i]);
           }
           else if (column_name == "name") {
               dog._name = std::string(argv[i]);
           }
           else if (column_name == "pathToPhoto") {
               dog._pictureLocation = std::string(argv[i]);
           }
           else if (column_name == "breed") {
               dog._bread = std::string(argv[i]);
           }
           else if (column_name == "age") {
               dog._age = std::stoi(argv[i]);
           }
           else if (column_name == "gender") {
               dog._gender = argv[i][0];
           }
       }
       ret->push_back(dog);
       return 0;
   };
   sqlite3* db = nullptr;
   int res = sqlite3_open(DB_NAME, &db);
   if (res)
   {
       std::cerr << "cant open db";
       exit(1);
   }
   res = sqlite3_exec(db, "select * from dogs;", get_dog_callback, &ret, nullptr);
   if (res)
   {
       std::cerr << "error with getting all dogs in db: " << sqlite3_errmsg(db);
       throw std::runtime_error("error in getting dogs out of db");
   }
   sqlite3_close(db);

   return ret;
}

bool Dog::DeleteDogById(int id)
{
    try
    {
        std::string query = "delete from dogs where id=";
        query += std::to_string(id);
        query += ';';
        sqlite3* db = nullptr;
       int res = sqlite3_open(DB_NAME, &db);
       if (res)
       {
           std::cerr << "cant open db";
           exit(1);
       }

       res = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
       if (res)
       {
           std::cerr << "cant delete dog: " << sqlite3_errmsg(db);
           throw res;
       }
       sqlite3_close(db);
       return true;
    }
    catch (...)
    {
        return false;
    }
}
