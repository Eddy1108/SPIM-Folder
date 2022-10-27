#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iterator>

#include "Core/VisualObject.h"
#include "mesh/Player/InteractiveObject.h"
#include "Core/CollisionShapes.h"


	typedef std::pair<double, double> Point2D;

    class QuadTre
    {
    private:

        //std::vector<VisualObject*> m_Objects;
        std::vector<VisualObject*> m_AllObjects;

        //std::list<std::pair<ID, T>> mObjects;
        //static std::vector<T> m_all_Objects;

        bool isLeaf() const;

        void traverse_all(std::vector<VisualObject*>& all_Objects);

    public:

        Point2D m_a;
        Point2D m_b;
        Point2D m_c;
        Point2D m_d;
        std::vector<VisualObject*> m_Objects;

        AABB* mShape{ nullptr };

        QuadTre* m_sw;
        QuadTre* m_se;
        QuadTre* m_ne;
        QuadTre* m_nw;


        QuadTre();
        QuadTre(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4);
        ~QuadTre();

        void init(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4);
        void subDivide(int n);
        void print();
        void print_all();
        void collisionCheck(glm::vec2 pos, std::string name);
        void removeEmpty();
        //void printCenter() const;
        //void printCorners() const;
        //void printAllLeaves() const;
        //Point2D findCenter() const;
        QuadTre* insert(VisualObject* gameObject);
        QuadTre* insert(VisualObject* gameObject, CollisionShape* shape);
        QuadTre* find(const glm::vec2 p);
        VisualObject* find(const std::string name);

        typename std::vector<VisualObject*>::iterator begin() 
        {
            traverse_all(m_AllObjects);
            return m_AllObjects.begin();
        }
        typename std::vector<VisualObject*>::iterator end()
        {
            return m_AllObjects.end();
        }

    };
