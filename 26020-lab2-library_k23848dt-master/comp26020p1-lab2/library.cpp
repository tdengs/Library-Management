#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "library.h"

#define SIZE_OF_BUFFER 100

using namespace std;

void Document::updateTitle(const char *newTitle){
    // resize _title with length of newTitle
    _title = (char*)realloc(_title, (strlen(newTitle) + 1) * sizeof(char));
    // check if memory reallocated successfully
    if (_title == NULL){
        printf("Requested memory failed to be reallocated\n");
        exit(0);
    }
    strcpy(_title, newTitle);
}

void Document::updateYear(int newYear){
    _year = newYear;
}

void Document::updateQuantity(int newQuantity){
    _quantity = newQuantity;
}

char *Document::getTitle(){
    return _title;
}

int Document::getYear(){
    return _year;
}

int Document::getQuantity(){
    return _quantity;
}

int Document::borrowDoc(){
    // check if the quantity of the document is more than 0
    if(getQuantity() > 0){
        updateQuantity(getQuantity() - 1);
        return 0;
    }
    return -1;
}

void Document::returnDoc(){
    updateQuantity(getQuantity() + 1);
}

//Novel
Novel::Novel(const char *title, const char *author, int year, int quantity){
    _title = (char*)malloc((strlen(title) + 1) * sizeof(char));
    // check if memory allocated successfully
    if(_title == NULL){
        printf("Requested memory failed to be allocated\n");
        exit(0);
    }
    strcpy(_title, title);
    _author = (char*)malloc((strlen(author) + 1) * sizeof(char));
    // check if memory reallocated successfully
    if(_author == NULL){
        printf("Requested memory failed to be allocated\n");
        exit(0);
    }
    strcpy(_author, author);
    _year = year;
    _quantity = quantity;
}

Novel::~Novel(){
    free(_author);
    free(_title);
}

void Novel::updateAuthor(const char *newAuthor){
    // resize _author with length of newAuthor
    _author = (char*)realloc(_author, (strlen(newAuthor) + 1) * sizeof(char));
    // check if memory reallocated successfully
    if (_author == NULL){
        printf("Requested memory failed to be reallocated\n");
        exit(0);
    }
    strcpy(_author, newAuthor);
}

char *Novel::getAuthor(){
    return _author;
}

document_type Novel::getDocType(){
    return DOC_NOVEL;
}

void Novel::print(){
    printf("Novel, title: %s, author: %s, year: %d, quantity: %d\n", _title, _author, _year, _quantity);
}

//Comic
Comic::Comic(const char *title, const char *author, int issue, int year, int quantity){
    _title = (char*)malloc((strlen(title) + 1) * sizeof(char));
    // check if memory allocated successfully
    if(_title == NULL){
        printf("Requested memory failed to be allocated\n");
        exit(0);
    }
    strcpy(_title, title);
    _author = (char*)malloc((strlen(author) + 1) * sizeof(char));
    // check if memory reallocated successfully
    if(_author == NULL){
        printf("Requested memory failed to be allocated\n");
        exit(0);
    }
    strcpy(_author, author);
    _issue = issue;
    _year = year;
    _quantity = quantity;
}

Comic::~Comic(){
    free(_author);
    free(_title);
}

void Comic::updateAuthor(const char *newAuthor){
    // resize _author with length of newAuthor
    _author = (char*)realloc(_author, (strlen(newAuthor) + 1) * sizeof(char));
    // check if memory reallocated successfully
    if (_author == NULL){
        printf("Requested memory failed to be reallocated\n");
        exit(0);
    }
    strcpy(_author, newAuthor);
}

char *Comic::getAuthor(){
    return _author;
}

void Comic::updateIssue(int newIssue){
    _issue = newIssue;
}

int Comic::getIssue(){
    return _issue;
}

document_type Comic::getDocType(){
    return DOC_COMIC;
}

void Comic::print(){
    printf("Comic, title: %s, author: %s, issue: %d, year: %d, quantity: %d\n", _title, _author, _issue, _year, _quantity);
}

//Magazine
Magazine::Magazine(const char *title, int issue, int year, int quantity){
    _title = (char*)malloc((strlen(title) + 1) * sizeof(char));
    // check if memory allocated successfully
    if(_title == NULL){
        printf("Requested memory failed to be allocated\n");
        exit(0);
    }
    strcpy(_title, title);
    _issue = issue;
    _year = year;
    _quantity = quantity;
}

Magazine::~Magazine(){
    free(_title);
}

void Magazine::updateIssue(int newIssue){
    _issue = newIssue;
}

int Magazine::getIssue(){
    return _issue;
}

document_type Magazine::getDocType(){
    return DOC_MAGAZINE;
}

void Magazine::print(){
    printf("Magazine, title: %s, issue: %d, year: %d, quantity: %d\n", _title, _issue, _year, _quantity);
}

//Library
Library::Library(){}

void Library::print(){
    for(int i = 0; i < _docs.size(); i++){
        _docs[i]->print();
    }
}

int Library::dumpCSV(const char *filename){
    int fd;
    // Initialise size of buffer to not get segmentation fault
    char buffer[SIZE_OF_BUFFER];
    // open file
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1){
        printf("Error with open\n");
        return -1;
    }

    for(int i = 0; i < _docs.size(); i++){
        document_type doc_type = _docs[i]->getDocType();

        if(doc_type == DOC_NOVEL){
            char *title = _docs[i]->getTitle();
            // Originally of type Document, need to cast it to Novel
            char *author = static_cast<Novel *>(_docs[i])->getAuthor();
            int year = _docs[i]->getYear();
            int quantity = _docs[i]->getQuantity();
            sprintf(buffer, "novel,%s,%s,,%d,%d\n", title, author, year, quantity);
            if(write(fd, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error writing to file\n");
                return -1;
            }
        }
        else if(doc_type == DOC_COMIC){
            char *title = _docs[i]->getTitle();
            // Originally of type Document, need to cast it to Comic
            char *author = static_cast<Comic *>(_docs[i])->getAuthor();
            int issue = static_cast<Comic *>(_docs[i])->getIssue();
            int year = _docs[i]->getYear();
            int quantity = _docs[i]->getQuantity();
            sprintf(buffer, "comic,%s,%s,%d,%d,%d\n", title, author, issue, year, quantity);
            if(write(fd, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error writing to file\n");
                return -1;
            }
        }
        else if(doc_type == DOC_MAGAZINE){
            char *title = _docs[i]->getTitle();
            // Originally of type Document, need to cast it to Magazine
            int issue = static_cast<Magazine *>(_docs[i])->getIssue();
            int year = _docs[i]->getYear();
            int quantity = _docs[i]->getQuantity();
            sprintf(buffer, "magazine,%s,,%d,%d,%d\n", title, issue, year, quantity);
            if(write(fd, buffer, strlen(buffer)) != strlen(buffer)){
                printf("Error writing to file\n");
                return -1;
            }
        }

    }
    close(fd);
    return 0;
}

Document *Library::searchDocument(const char *title){
    for(int i = 0; i < _docs.size(); i++){
        // check if any of the documents in vector _docs has a matching title
        if(!strncmp(_docs[i]->getTitle(), title, strlen(title) * sizeof(char))){
            return _docs[i];
        }
    }
    return NULL;
}

int Library::addDocument(Document *d){
    // check if document already exist
    if(searchDocument(d->getTitle()) != NULL){
        return -1;
    }
    // add document
    _docs.push_back(d);
    return 0;
}

int Library::delDocument(const char *title){
    for(int i = 0; i < _docs.size(); i++){
        // check if any of the documents in vector _docs has a matching title
        if(!strncmp(_docs[i]->getTitle(), title, strlen(title) * sizeof(char))){
            // delete document
            _docs.erase(_docs.begin() + i);
            return 0;
        }
    }
    return -1;
}

int Library::countDocumentOfType(document_type t){
    int count = 0;
    for(int i = 0; i < _docs.size(); i++){
        if(_docs[i]->getDocType() == t){
            count++;
        }
    }
    return count;
}

int Library::borrowDoc(const char *title){
    Document *document = searchDocument(title);
    // No document with corresponding title
    if(document == NULL){
        return -1;
    }
    else{
        // call the document's borrowDoc function
        if(document->borrowDoc() == 0){
            return 0;
        }
    }
    return -1;
}

int Library::returnDoc(const char *title){
    Document *document = searchDocument(title);
    // No document with corresponding title
    if(searchDocument(title) == NULL){
        return -1;
    }
    // call the document's returnDoc function
    document->returnDoc();
    return 0;

}
