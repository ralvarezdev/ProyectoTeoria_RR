#include <assert.h>

#include "../namespaces.h"
#include "../terminal/input.h"

using namespace clients;
using namespace commands;
using namespace movies;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
clientStatus getClientId(Clients *clients, int *id, int *index, string message);
void getClients(Clients *clients);
void overwriteClients(Clients *clients);
void addClientToFile(Clients *clients);
void createClientWithId(Clients *clients, Client newClient, int *index);
void filterClientsData(Clients *clients, string **params, bool fields[], int sortBy[]);
void sortClients(Clients *clients, int sortBy[], int n);
void clientsMergeSort(Clients *clients, int sortByIndex);
void storeMovieMovement(string time, int clientId, int movieId, bool isRent);

// --- Templates

template <typename T>

// Function to Check if Client Unique Fields have been Ocuppied
clientStatus checkClient(Clients *clients, T unique, cmdClientFields field, int *index)
{
  if (field != clientFieldId && field != clientFieldAccountNumber)
    return clientErrorStatus;
  else
    clientsMergeSort(clients, field * 2); // Sort Clients by ID or by Account Number

  if ((*clients).getNumberClients() == 0)
    return clientNotFound;

  Client client;

  bool found = false;
  T value;
  int mid, start = 0, end = (*clients).getNumberClients() - 1;
  string line;

  assert(end >= start); // Check if the Last Index of the Array is Greater than the First One

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      client = (*clients).getClient(mid); // Get Client at the Given Index

      if (field == clientFieldId)
        value = client.id;
      else if (field == clientFieldAccountNumber)
        value = client.account;

      found = (value == unique);
    }
    catch (...)
    {
      if (field == clientFieldId)
        wrongClientData(invalidClientId);
      else if (field == clientFieldAccountNumber)
        wrongClientData(invalidClientAccountNumber);
    }

    if (found)
    {
      if (field == clientFieldId)
        *index = mid;
      else if (field == clientFieldAccountNumber)
        checkClient(clients, client.id, clientFieldId, index); // Get Index of Client with that Account Number when the Array is Sorted by ID

      return clientFound;
    }
    else if (unique > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return clientNotFound;
}

#endif