// https://contest.yandex.ru/contest/17453/run-report/33286098/


#include <algorithm>
#include <optional>
#include <iostream>
#include <functional>
#include <vector>
#include <set>
#include <sstream>


template <class T>
class Cell {
public:

	Cell<T>* Previous;
	Cell<T>* Next;
	std::optional<T> Value;

	Cell(Cell<T>* prev, Cell<T>* next) {
		Previous = prev;
		Next = next;
	}

	Cell(const Cell& other) {
		if (*this != other) {
			Previous = other.Previous;
			Next = other.Next;
			Value = other.Value;
		}
	}

	Cell(Cell&& other) {
		if (*this != other) {
			Previous = other.Previous;
			Next = other.Next;
			Value = other.Value;
		}
	}

	Cell& operator = (const Cell& other) {
		if (*this != other) {
			Previous = other.Previous;
			Next = other.Next;
			Value = other.Value;
		}
	}

	Cell& operator = (Cell&& other) {
		if (*this != other) {
			Previous = other.Previous;
			Next = other.Next;
			Value = other.Value;
		}
	}

	Cell(Cell<T>* prev, Cell<T>* next, T&& value) {
		Previous = prev;
		Next = next;
		Value = std::move(value);
	}

	Cell(Cell<T>* prev, Cell<T>* next, const T& value) {
		Previous = prev;
		Next = next;
		Value = value;
	}

};


template <class T>
class List {
public:

	using value_type = T;

	class Iter : public std::iterator <std::bidirectional_iterator_tag, T> {

		friend class List<T>;

	public:
		using value_type = T;
		Iter(const Iter& it) : CellElement(it.CellElement) { }

		Iter(Cell<T>* element) {
			CellElement = element;
		}

		Iter& operator ++ () {
			CellElement = CellElement->Next;
			return *this;
		}

		Iter& operator -- () {
			CellElement = CellElement->Previous;
			return *this;
		}

		Iter operator ++ (int) {
			Iter res = Iter(CellElement);
			CellElement = CellElement->Next;
			return res;
		}

		Iter operator -- (int) {
			Iter res = Iter(CellElement);
			CellElement = CellElement->Previous;
			return res;
		}

		T& operator * () {
			return *CellElement->Value;
		}

		bool operator != (const Iter& other) const {
			return CellElement != other.CellElement;
		}

		bool operator == (const Iter& other) const {
			return CellElement == other.CellElement;
		}

		Cell<T>* getCell() const {
			return CellElement;
		}

		Cell<T>* CellElement;
	};

	class CIter : public std::iterator <std::bidirectional_iterator_tag, T> {

		friend class List<T>;

	public:

		CIter(const CIter& it) {
			CellElement = it.CellElement;
		}

		CIter(const Iter& it) {
			CellElement = it.CellElement;
		}

		CIter(Cell<T>* element) {
			CellElement = element;
		}

		CIter& operator ++ () {
			CellElement = CellElement->Next;
			return *this;
		}

		CIter operator ++ (int) {
			Iter res = Iter(CellElement);
			CellElement = CellElement->Next;
			return res;
		}

		CIter& operator -- () {
			CellElement = CellElement->Previous;
			return *this;
		}

		bool operator != (const CIter& other) const {
			return !(*this == other);
		}

		bool operator == (const CIter& other) const {
			return CellElement == other.CellElement;
		}
		const T& operator * () {
			return *CellElement->Value;
		}

		Cell<T>* getCell() const {
			return CellElement;
		}

		Cell<T>* CellElement;
	};

	typedef Iter iterator;

	List() {
		Head = nullptr;
		Tail = nullptr;
		FinishElement = new Cell<T>(nullptr, nullptr);
		FinishElement->Previous = Tail;
		Size = 0;
	}

	List(const List& other) {
		Head = nullptr;
		Tail = nullptr;
		Size = 0;
		FinishElement = new Cell<T>(nullptr, nullptr);
		for (auto x : other) {
			insert(end(), x);
		}
		if (Tail != nullptr) {
			Tail->Next = FinishElement;
			FinishElement->Previous = Tail;
		}
	}

	List(size_t count, const T& value = T()) : List() {
		for (size_t i = 0; i < count; ++i) {
			insert(end(), value);
		}
	}

	List(List&& other) noexcept {
		Head = other.Head;
		Tail = other.Tail;
		Size = other.Size;
		FinishElement = new Cell<T>(nullptr, nullptr);
		if (Tail != nullptr) {
			Tail->Next = FinishElement;
			FinishElement->Previous = Tail;
		}
		other.Head = other.Tail = nullptr;
		other.Size = 0;
	}

	List<T>& operator = (const List& other) {

		if (this != &other) {
			while (Size > 0) {
				pop_front();
			}
			Size = 0;
			for (auto it = other.cbegin(); it != other.cend(); ++it) {
				push_back(*it);
			}
			if (!FinishElement) {
				FinishElement = new Cell<T>(nullptr, nullptr);
			}
			if (Tail != nullptr) {
				Tail->Next = FinishElement;
				FinishElement->Previous = Tail;
			}
		}

		return *this;
	}

	List<T>& operator = (List&& other) noexcept {

		if (&other != this) {
			Tail = other.Tail;
			Head = other.Head;
			Size = other.Size;
			if (!FinishElement) {
				FinishElement = new Cell<T>(nullptr, nullptr);
			}
			if (Tail != nullptr) {
				Tail->Next = FinishElement;
				FinishElement->Previous = Tail;
			}
			other.Head = other.Tail = nullptr;
			other.Size = 0;
		}

		return *this;
	}

	~List() {
		while (Size > 0) {
			pop_back();
		}
		delete FinishElement;
	}

	size_t size() {
		return Size;
	}

	T& front() {
		return *Head->Value;
	}

	T& back() {
		return *Tail->Value;
	}

	void clear() {

		while (Size > 0) {
			pop_back();
		}
		Head = nullptr;
		Tail = nullptr;
		FinishElement->Previous = nullptr;

	}

	bool empty() {
		return Size == 0;
	}

	Iter insert(CIter it, T&& value) {
		auto tmp = it.getCell();
		if (Size == 0) {
			auto newElement = new Cell<T>(nullptr, nullptr, std::forward<T>(value));
			Head = newElement;
			Tail = newElement;
			Head->Next = FinishElement;
			FinishElement->Previous = Head;
			incrementSize();
			return Iter(newElement);
		}
		auto newElement = new Cell<T>(nullptr, nullptr, std::forward<T>(value));
		if (tmp == FinishElement) {
			return addElement(newElement);
		}
		if (tmp == Head) {
			newElement->Next = Head;
			Head->Previous = newElement;
			Head = newElement;
			incrementSize();
			return Iter(newElement);
		}
		else {
			newElement->Next = tmp;
			newElement->Previous = tmp->Previous;
			changeRelations(tmp, newElement);
			incrementSize();
			return Iter(newElement);
		}
	}

	Iter addElement(Cell<T>* newElement) {
		newElement->Previous = Tail;
		newElement->Next = FinishElement;
		FinishElement->Previous = newElement;
		Tail->Next = newElement;
		Tail = newElement;
		incrementSize();
		return Iter(newElement);
	}

	Iter insert(CIter it, const T& value) {
		auto tmp = it.getCell();
		auto newElement = new Cell<T>(nullptr, nullptr, value);
		if (Size == 0) {
			Head = newElement;
			Tail = newElement;
			Head->Next = FinishElement;
			FinishElement->Previous = Head;
			incrementSize();
			return Iter(newElement);
		}
		if (tmp == FinishElement) {
			newElement->Previous = Tail;
			newElement->Next = FinishElement;
			FinishElement->Previous = newElement;
			Tail->Next = newElement;
			Tail = newElement;
			incrementSize();
			return Iter(newElement);
		}
		if (tmp == Head) {
			newElement->Next = Head;
			Head->Previous = newElement;
			Head = newElement;
			incrementSize();
			return Iter(newElement);
		}
		else {
			newElement->Next = tmp;
			newElement->Previous = tmp->Previous;
			changeRelations(tmp, newElement);
			if (tmp == Tail) {
				Tail = newElement;
			}
			incrementSize();
			return Iter(newElement);
		}

	}

	void changeRelations(Cell<T>*& tmp, Cell<T>*& newElement) {
		tmp->Previous->Next = newElement;
		tmp->Previous = newElement;
	}

	void push_back(T&& value) {
		insert(end(), std::forward<T>(value));
	}

	void push_back(const T& value) {
		insert(end(), value);
	}

	void push_front(T&& value) {
		insert(begin(), std::forward<T>(value));
	}

	void push_front(const T& value) {
		insert(begin(), value);
	}

	void pop_back() {
		erase(--end());
	}

	void pop_front() {
		erase(begin());
	}

	template <class ... Args>
	Iter emplace(CIter it, Args&& ... args) {
		//auto tmp = it.getCell();
		insert(CIter(it), std::forward<T>(T(std::forward<Args ...>(args...))));
		return Iter(it.CellElement->Previous);
	}

	template <class ... Args>
	void emplace_back(Args&& ... args) {
		emplace(cend(), std::forward<Args>(args)...);
	}

	template <class ... Args>
	void emplace_front(Args&& ... args) {
		emplace(cbegin(), std::forward<Args>(args)...);
	}

	void erase(Iter it) {
		if (it == --end()) {
			if (Size < 0) {
				return;
			}
			if (Size == 2) {
				auto temp = Tail;
				Tail = Head;
				Tail->Next = FinishElement;
				FinishElement->Previous = Tail;
				decrementSize();
				delete temp;
				return;
			}
			if (Size > 2) {
				auto TailPrev = Tail->Previous;
				delete Tail;
				Tail = TailPrev;
				TailPrev->Next = FinishElement;
				FinishElement->Previous = Tail;
				decrementSize();
				return;
			}
			delete Head;
			Head = nullptr;
			Tail = nullptr;
			decrementSize();
			return;
		}
		auto tmp = it.getCell();
		if (tmp == Head) {
			Head = tmp->Next;
			Head->Previous = nullptr;
		}
		else {
			tmp->Previous->Next = tmp->Next;
			tmp->Next->Previous = tmp->Previous;
		}
		decrementSize();
		if (Size <= 1) {
			Tail = Head;
		}
		delete tmp;
	}

	void erase(Iter it_start, Iter it_end) {
		while (it_start != it_end) {
			auto tmp = it_start.getCell();
			++it_start;
			if (tmp == Head) {
				Head = tmp->Next;
				Head->Previous = nullptr;
			}
			else {
				tmp->Previous->Next = tmp->Next;
				tmp->Next->Previous = tmp->Previous;
			}
			decrementSize();
			if (Size <= 1) {
				Tail = Head;
			}
			delete tmp;
		}
	}

	void decrementSize() {
		--Size;
	}

	void incrementSize() {
		++Size;
	}

	void reverse() {
		std::reverse(begin(), end());
	}

	void unique() {
		if (Size <= 1) {
			return;
		}
		auto it = begin();
		while (it != --end()) {
			auto tmp = it;
			if (*tmp == *++it) {
				erase(it);
			}
			else {
				++tmp;
			}
			it = tmp;
		}
	}

	CIter cend() const {
		return CIter(FinishElement);
	}

	CIter cbegin() const {
		if (Size == 0) {
			return cend();
		}
		return CIter(Head);
	}

	Iter end() const {
		return Iter(FinishElement);
	}

	Iter begin() const {
		if (Size == 0) {
			return end();
		}
		return Iter(Head);
	}

	Cell<T>* Head;
	Cell<T>* Tail;
	Cell<T>* FinishElement;
	size_t Size;

};