/*
 Name: Anthony Rutherford
 Email: adrutherford@crimson.ua.edu
 Course Section: Fall 2023 CS 201
 Homework #: 3
 Instructions to Compile: g++ hw3.cpp
 Instructions to Execute: ./a.exe inputFilename queryFilename
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <unordered_map>
 
using namespace std;

int main(int argc, char **argv) {
//    Program Initialization
//-------------------------------------------------------------------------------------------------
    string inputName = argv[1];
    string queryName = argv[2];
    ifstream input, query;
    ofstream output; 
    input.open(inputName);  //open input file for reading in data
    output.open("output.txt");  //print to output file faster than std::cout
    unordered_map<string, vector<string>> moviesMap;  //set up table for searching movie
    unordered_map<string, vector<string>> actorsMap;  //set up table for searching actor
    auto start_time_map = std::chrono::high_resolution_clock::now();

//    Building Data Stucture (unordered_map / Hash Table) for program
//---------------------------------------------------------------------------------------------
    while(1){
        string hold;
        bool titleFound = false;  // used by parser to know how to save curr
        vector<string> titleVector;  // single vector holding title for adding actors to actor map
        vector<string> actors;      // list of actors
        string title; 
        string curr = "";
        getline(input, hold);
        string accountForNewLine = hold;  //using to account for newline character from getline
        hold.pop_back();                    //pops new line character
        if(input.eof()) hold = accountForNewLine;  //last line does not have new line character so do not pop
        for (int i = 0; i < hold.length(); i++){
            if (hold.at(i) != '/'){   //parse line looking for '/' as seperator
                curr = curr + hold.at(i);  //add character to current phrase if not '/'
            }
            else{
                if(!titleFound){  //check if title has been found so we know if it is title or actor
                    title = curr;
                    titleVector.push_back(title);
                    titleFound = true;
                    curr = "";
                }
                else{ // title is found so curr is an actor 
                    actors.push_back(curr);
                    if (actorsMap.find(curr) != actorsMap.end()){  //if actor is already in map add title
                        actorsMap[curr].push_back(title);
                    }
                    else{   // if actor is not in actor map add
                        actorsMap.insert({curr,titleVector});
                    }
                    curr = "";    // reset current word
                }
            
            }
        }
        actors.push_back(curr);   //since line does not end with '/' add actor when line is over
        if (actorsMap.find(curr) != actorsMap.end()){
            actorsMap[curr].push_back(title);
        }
        else{
            actorsMap.insert({curr,titleVector});
        }
        moviesMap.insert({title,actors});
        if(input.eof()) break;                
    }
    auto end_time_map = std::chrono::high_resolution_clock::now();
    input.close();

//  Search Logic For Program
//---------------------------------------------------------------------------------------
    query.open(queryName);
    auto start_time_search = std::chrono::high_resolution_clock::now();
    while(1){
        string searching;
        if(query.eof()) break;
        getline(query, searching);
        string noNewLine = searching; // similar to previous used to account for varying newline
        searching.pop_back();  // pops new line
        if(query.eof()) searching = noNewLine;  // does not pop newline if last line
        if (searching.at(searching.size() - 1) == ')'){  //checks which table to search for using movie title structure having an ')' last
            if (moviesMap.find(searching) == moviesMap.end()){   //if movie not found print message
             output << "Movie not found." << endl;
            }
            else{
                for (int i = 0; i < moviesMap[searching].size(); i++){  // print list of actors
                    output << moviesMap[searching].at(i) << endl;
                }
            }
        }
        else{  // if string does not end with ')' then it is an actor
            if (actorsMap.find(searching) == actorsMap.end()){  // if actor not found print message
                output << "Actor not found." << endl;
            }
            else{
                for (int i = 0; i < actorsMap[searching].size(); i++){  // print list of movies 
                    output << actorsMap[searching].at(i) << endl;
                }
            }
        }
        if(query.eof()) break;
    }

//   Print Final Results
//------------------------------------------------------------------------------
    auto end_time_search = std::chrono::high_resolution_clock::now();
    query.close();
    output.close();

    auto duration_map = std::chrono::duration_cast<std::chrono::microseconds>(end_time_map - start_time_map).count();
    auto duration_search = std::chrono::duration_cast<std::chrono::microseconds>(end_time_search - start_time_search).count();
    output.close();
    cout << "Size: " << moviesMap.size() << endl;
    cout << "Build Time: " << duration_map << " microseconds" << endl;
    cout << "Search Time: " << duration_search << " microseconds" << endl;
    cout << "Total Time: " << duration_map + duration_search << " microseconds" << endl;
    return 0;
}
