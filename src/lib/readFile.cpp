#include <iostream>
#include <fstream>
#include <sstream>
#include "data.h"
#include "readFile.h"
#include "input.h"

using namespace std;

// --- Extern Variables and Constants Definition
extern const int nMaxGenres = 5; // Max Number of Genres Per Movie

// --- Global Variables
int nMoviesRead; // Number of Movies that had been Read and Copied from movies.csv

// --- Functions Prototypes
int getMovies(Movie movies[], int m, bool fields[], int n);
void getGenres(string word, int genres[], int n);
void getDate(string word, int date[], int n);
void getDirector(string word, string director[], int n);
void sortMovies(Movie *movies, int sortBy[], int n);
void moviesMergeSort(Movie *movies, int n, int sortByIndex);
void moviesMerge(Movie *movies, Movie *sorted, int low, int mid, int high, int sortByIndex);
// string getLower(string word);

// Function to Get an Array of Movies from movies.csv
int getMovies(Movie movies[], int m, bool fields[], int n)
{
  int genres[genreEnd], date[3];
  string director[2];
  int count = 0, nline = 0;
  string line, word;

  ifstream infile("movies.csv", ifstream::in);

  if (!infile.is_open()) // Couldn't Access to infile
    pressEnterToContinue("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  else
  {
    getline(infile, line); // Remove First Line, which is Used as a Title

    while (getline(infile, line))
    {
      fill(genres, genres + genreEnd, -1); // Initialize Genres Array

      stringstream file(line);

      while (getline(file, word, ';'))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            movies[nline].id = stoi(word);
            break;
          case 1:
            movies[nline].name = word;
            break;
          case 2:
            getGenres(word, genres, nMaxGenres);
            for (int i = 0; i < genreEnd; i++)
              movies[nline].genres[i] = genres[i];
            break;
          case 3:
            movies[nline].duration = stoi(word);
            break;
          case 4:
            getDirector(word, director, 2);
            for (int i = 0; i < 2; i++)
              movies[nline].director[i] = director[i];
            break;
          case 5:
            movies[nline].price = stof(word);
            break;
          case 6:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              movies[nline].releaseDate[i] = date[i];
            break;
          case 7:
            movies[nline].rentTo = stoi(word); // Client ID
            break;
          case 8:
            getDate(word, date, 3);
            for (int i = 0; i < 3; i++)
              movies[nline].rentOn[i] = date[i]; // Rent Date
            break;
          case 9:
            movies[nline].rentStatus = stoi(word); // Rent Status (Yes for Rented)
            break;
          }
        count++;
      }
      nline++;
      count = 0;
    }
    nMoviesRead = nline;
    infile.close();
  }
  return nMoviesRead;
}

// Function to Get Movie Genres
void getGenres(string word, int genres[], int n)
{
  int counter = 0;
  string genre;

  stringstream stream(word);
  while (getline(stream, genre, '|') && counter < n) // Get Genre
  {
    for (int i = 0; i < genreEnd; i++)
    {
      if (genre == genrePtr[i] || i == errorGenre)
        genres[counter] = i;
      else
        continue;
      break;
    }
    counter++;
  }
}

// Function to Get Date
void getDate(string word, int date[], int n)
{
  int counter = 0;
  string temp;

  stringstream stream(word);
  while (getline(stream, temp, '-') && counter < n) // Get Date
  {
    date[counter] = stoi(temp);
    counter++;
  }
}

// Function to Get Director First and Last Name
void getDirector(string word, string director[], int n)
{
  string temp;
  int counter = 0;

  stringstream stream(word);
  while (getline(stream, temp, ' ') && counter < n)
  { // Get Director Name
    director[counter] = temp;
    counter++;
  }
}

// Function to Sort Movies (Uses Merge Sort)
void sortMovies(Movie *movies, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      moviesMergeSort(movies, nMoviesRead, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void moviesMergeSort(Movie *movies, int n, int sortByIndex)
{
  int pass, low, high, mid, i;
  Movie *sorted = new Movie[n]; // Store the Array in the Heap Memory

  for (pass = 2; pass <= n; pass *= 2)
  {
    for (i = 0; i + pass - 1 < n; i += pass)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      moviesMerge(movies, sorted, low, mid, high, sortByIndex);
    }
    if (n - i > pass / 2)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      moviesMerge(movies, sorted, low, mid, n - 1, sortByIndex);
    }
  }

  if (pass / 2 < n)
    moviesMerge(movies, sorted, 0, pass / 2 - 1, n - 1, sortByIndex);

  delete[] sorted; // Deallocate Memory

  if (sortByIndex % 2 != 0) // True if it's an Odd Number, which is Used for Descending Order
  {
    int j = 0;
    Movie temp;

    for (i = n - 1; i > (n - 1) / 2; i--)
    {
      temp = movies[j];
      movies[j] = movies[i];
      movies[i] = temp;
      j++;
    }
  }
}

// Function to Merge Movies Subarrays
void moviesMerge(Movie *movies, Movie *sorted, int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  string firstTitle, secondTitle; // Used when Sorting the Array by Title
  bool op;                        // Boolean Operator

  switch (sortByIndex / 2)
  {
  case sortByDurationA / 2:
    while (i <= mid && j <= high)
      if (movies[i].duration < movies[j].duration)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case sortByIdA / 2:
    while (i <= mid && j <= high)
      if (movies[i].id < movies[j].id)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case sortByPriceA / 2:
    while (i <= mid && j <= high)
      if (movies[i].price < movies[j].price)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    break;

  case sortByTitleA / 2:
    while (i <= mid && j <= high)
    {
      firstTitle = movies[i].name;
      secondTitle = movies[j].name;

      if (firstTitle.compare(secondTitle) < 0)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
    }
    break;

  case sortByReleaseA / 2:
    while (i <= mid && j <= high)
    {
      if (movies[i].releaseDate[0] != movies[j].releaseDate[0])
        op = movies[i].releaseDate[0] > movies[j].releaseDate[0];
      else if (movies[i].releaseDate[1] != movies[j].releaseDate[1])
        op = movies[i].releaseDate[1] > movies[j].releaseDate[1];
      else if (movies[i].releaseDate[2] != movies[j].releaseDate[2])
        op = (movies[i].releaseDate[2] > movies[j].releaseDate[2]);
      else
        op = false;

      if (op)
        sorted[k++] = movies[i++];
      else
        sorted[k++] = movies[j++];
      break;
    }

    while (i <= mid)
      sorted[k++] = movies[i++];
    while (j <= high)
      sorted[k++] = movies[j++];

    for (i = low; i <= high; i++)
      movies[i] = sorted[i];
  }

  /*
  // Function to Get a Lowercase String
  string getLower(string word)
  {
    string wordToLower;

    for (int i = 0; i < word.length(); i++)
      wordToLower += tolower(word[i]); // Append Character in Lowercase

    return wordToLower;
  }
  */
