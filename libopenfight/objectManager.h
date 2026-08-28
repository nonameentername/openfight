#ifndef OPEN_FIGHT_OBJECT_MANAGER_H
#define OPEN_FIGHT_OBJECT_MANAGER_H

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>

template <class T> class CompareObject {
public:
    bool operator()(const T *l, const T *r) {
        return *l < *r;
    }
};

template <class T> class ObjectManager {
private:
    std::map<std::string, T *> objects;
    typename std::map<std::string, T *>::iterator i;
    std::list<T *> sortedList;

public:
    ObjectManager() {
    }

    ObjectManager(ObjectManager<T> &o) {
        typename std::map<std::string, T *>::const_iterator i = o.objects.begin();

        for (i = o.objects.begin(); i != o.objects.end(); i++)
            objects[i->first] = new T(*i->second);
    }

    ~ObjectManager() {
        typename std::map<std::string, T *>::iterator i;

        for (i = objects.begin(); i != objects.end(); i++)
            delete i->second;
    }

    void add(std::string name, T *object) {
        if (!contains(name))
            objects[name] = object;
        else
            delete object;
    }

    bool contains(std::string name) {
        return objects.find(name) != objects.end();
    }

    void remove(std::string name) {
        delete objects[name];
        objects.erase(name);
    }

    T *get(std::string name) {
        return objects[name];
    }

    std::string first() {
        i = objects.begin();

        if (i != objects.end())
            return i->first;

        return "";
    }

    std::string next() {
        i++;

        if (i != objects.end())
            return i->first;

        return "";
    }

    std::list<T *> getSortedList() {
        sortedList.clear();

        for (i = objects.begin(); i != objects.end(); i++)
            sortedList.push_back(i->second);

        sortedList.sort(CompareObject<T>());

        return sortedList;
    }

    void clear() {
        typename std::map<std::string, T *>::iterator i;

        for (i = objects.begin(); i != objects.end(); i++)
            delete i->second;

        objects.clear();
    }
};

#endif
