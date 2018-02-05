/*
 * IList.h
 *
 *  Created on: 07/03/2016
 *      Author: EDI/DIS teachers
 */

#ifndef ILIST_H_
#define ILIST_H_


#include <iostream>
using namespace std;


template <class TYPE>
class IList {

private:

	class Node {

	public:

		TYPE   elem;
		Node  *next;
		Node  *prev;

		Node (const TYPE &elem) {
			this->elem  = elem;
			this->next  = NULL;
			this->prev  = NULL;
		}
	};

	Node *head;
	Node *tail;
	Node *poi;


public:

    /* DESC:   Creates an iterator list empty
     * PRE:    -
     * POST:   Post = { empty(ilist) = true }
     * PARAM:  -
     * RET:    -
     * COMP:   O(1)
     */
	IList ();

    /* DESC:   Is the list empty?
     * PRE:    Pre  = {}
     * POST:   Post = {}
     * PARAM:  -
     * RET:    True:  empty list
     *         FALSE: not empty list
     * COMP:   O(1)
     */
	bool  empty ();

    /* DESC:   Put the POI in the head element of the list
     * PRE:    Pre  = {}
     * POST:   Post = {consult(poi) = head}
     * PARAM:  -
     * RET:    -
     * COMP:   O(1)
     */
	void  moveToBegin ();

	/* DESC:   Put the POI in the tail element of the list
     * PRE:    Pre  = {}
     * POST:   Post = {consult(poi) = tail}
     * PARAM:  -
     * RET:    -
     * COMP:   O(1)
     */
	void  moveToEnd ();

    /* DESC:   Put the POI to the next element of the list
     * PRE:    Pre  = {}
     * POST:   Post = {}
     * PARAM:  -
     * RET:    -
     * COMP:   O(1)
     */
	void  next ();

    /* DESC:   Put the POI to the previous element of the list
     * PRE:    Pre  = {}
     * POST:   Post = {}
     * PARAM:  -
     * RET:    -
     * COMP:   O(1)
     */
	void  back ();

    /* DESC:   Is the POI the head element?
     * PRE:    Pre  = {}
     * POST:   -
     * PARAM:  -
     * RET:    True:  consult(poi) = head
     *         False: consult(poi) != head
     * COMP:   O(1)
     */
	bool  first ();

    /* DESC:   Is the POI the tail element?
     * PRE:    Pre  = {}
     * POST:   -
     * PARAM:  -
     * RET:    True:  consult(poi) = tail
     *         False: consult(poi) != tail
     * COMP:   O(1)
     */
	bool  last ();

    /* DESC:   Is the POI tha next(tail) element?
     * PRE:    Pre  = {}
     * POST:   -
     * PARAM:  -
     * RET:    True:  consult(poi) = next(tail)
     *         False: consult(poi) != next(tail)
     * COMP:   O(1)
     */
	bool  end ();

    /* DESC:   Insert an element in the iterator list, just before the POI element
     * PRE:    Pre  = {}
     * POST:   Post = {consult(POI) = elem}
     * PARAM:  Input: elem -> Element to insert
     * RET:    -
     * COMP:   O(1)
     */
	void  insert (const TYPE &elem);

	/* DESC:   Return the POI element
	 * PRE:    Pre  = {empty(ilist) = false}
	 * POST:   -
	 * PARAM:  Output: elem -> POI element
	 * RET:    -
	 * COMP:   O(1)
	 */
	void  consult (TYPE &elem);

	/* DESC:   Remove the POI element
	 * PRE:    Pre  = {empty(ilist) = false}
	 * POST:   Post = {consult(POI) = next}
	 * PARAM:  -
	 * RET:    -
	 * COMP:   O(1)
	 */
	void  remove ();

	/* DESC:   Deallocate memory of the nodes
	 * PRE:    Pre  = {}
	 * POST:   Post = {empty(ilist) = true}
	 * PARAM:  -
	 * RET:    -
	 * COMP:   O(n)
	 */
     ~IList();

};




template <class TYPE>
IList<TYPE>::IList () {
	this->head  = NULL;
	this->tail  = NULL;
	this->poi   = NULL;
}

template <class TYPE>
bool IList<TYPE>::empty () {
	return (this->head == NULL);
}

template <class TYPE>
void IList<TYPE>::moveToBegin (){
	this->poi= this->head;
}

template <class TYPE>
void IList<TYPE>::moveToEnd (){
	this->poi = this->tail;
}

template <class TYPE>
void IList<TYPE>::next () {

	if (this->poi!= NULL) {
		this->poi= this->poi->next;
	}
}

template <class TYPE>
void IList<TYPE>::back (){

	if (this->poi!= this->head) {
		if (this->poi!= NULL) {
			this->poi= this->poi->prev;
		} else {
			this->poi = this->tail;
		}
	}
}

template <class TYPE>
bool IList<TYPE>::first (){
	return (this->poi == this->head);
}

template <class TYPE>
bool IList<TYPE>::last (){
	return (this->poi == this->tail);
}

template <class TYPE>
bool IList<TYPE>::end (){
	return (this->poi == NULL);
}

template <class TYPE>
void IList<TYPE>::consult (TYPE &elem){

	if (this->poi!= NULL) {
		elem = this->poi->elem;
	}
}

template <class TYPE>
void IList<TYPE>::insert (const TYPE &elem) {


	Node *aux = new Node (elem);

	if (this->poi == NULL){

		if (empty()) { // Lista vacia
			aux->next = this->poi;
			this->head = aux;
			this->tail    = aux;
		} else { // POI points to the next element to the tail. Insert to the last position
			aux->prev = this->tail;
			this->tail->next  = aux;
			this->tail = aux;
		}

	} else {

		if (this->poi->prev == NULL) { // Insert at the beginning
			this->head = aux;
			aux->next = this->poi;
			aux->prev = NULL;
			this->poi->prev = aux;
		} else {  // Insert in an intermediate position
			aux->prev  = this->poi->prev;
			aux->next = this->poi;
			(this->poi->prev)->next = aux;
			this->poi->prev = aux;
		}

	}

	// POI points to the new element
	this->poi= aux;
}

template <class TYPE>
void IList<TYPE>::remove () {

	Node *aux;

	if (!empty() && !end()) {

		aux = this->poi;

		if (this->poi == this->head) // Remove the first node (head)
			this->head = this->head->next;
		if (this->poi == this->tail)    // Remove the last node (tail)
			this->tail = this->tail->prev;

		this->poi= this->poi->next; // POI to the next element

		if (aux->prev!=NULL)
			(aux->prev)->next = this->poi;
		if (aux->next!=NULL)
			this->poi->prev = aux->prev;

		delete aux;
	}
}

template <class TYPE>
IList<TYPE>::~IList() {

	moveToBegin();
	while (!empty())
		remove ();

	this->head = NULL;
	this->tail = NULL;
	this->poi  = NULL;
}




#endif /* ILIST_H_ */
