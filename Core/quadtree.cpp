#include "quadtree.h"


bool QuadTre::isLeaf() const
{
    return m_sw == nullptr && m_se == nullptr && m_ne == nullptr && m_nw == nullptr;
}


void QuadTre::traverse_all(std::vector<VisualObject*>& all_Objects)
{
    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
    {
        all_Objects.push_back(*it);
    }
    if (m_ne)
    {
        m_ne->traverse_all(all_Objects);
    }
    if (m_nw)
    {
        m_nw->traverse_all(all_Objects);
    }
    if (m_se)
    {
        m_se->traverse_all(all_Objects);
    }
    if (m_sw)
    {
        m_sw->traverse_all(all_Objects);
    }
}



QuadTre::QuadTre() : m_sw{ nullptr }, m_se{ nullptr }, m_ne{ nullptr }, m_nw{ nullptr }
{
    mShape = new AABB();

    glm::vec2 min{ m_a.first, m_a.second };
    glm::vec2 max{ m_c.first, m_c.second };
    glm::vec2 mid{ (min.x + max.x) / 2, (min.y + max.y) / 2 };

    mShape->mExtent = glm::vec3{ abs(max.x - mid.x) };
    mShape->mPosition = glm::vec3{ mid.x, mid.y, 0.f };
}


QuadTre::QuadTre(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4) : m_a{ v1 }, m_b{ v2 }, m_c{ v3 }, m_d{ v4 },
m_sw{ nullptr }, m_se{ nullptr }, m_ne{ nullptr }, m_nw{ nullptr }
{
    mShape = new AABB();

    glm::vec2 min{ m_a.first, m_a.second };
    glm::vec2 max{ m_c.first, m_c.second };
    glm::vec2 mid{ (min.x + max.x) / 2, (min.y + max.y) / 2 };

    mShape->mExtent = glm::vec3{ abs(max.x - mid.x) };
    mShape->mPosition = glm::vec3{ mid.x, mid.y, 0.f };
}

QuadTre::~QuadTre()
{
    if (m_sw)
        delete m_sw;

    if (m_se)
        delete m_se;

    if (m_nw)
        delete m_nw;

    if (m_ne)
        delete m_ne;

}


void QuadTre::init(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
{
    m_a = v1; m_b = v2; m_c = v3; m_d = v4;
}

void QuadTre::subDivide(int n)
{
    if (n > 0)
    {
        //std::cout << "N = " << n << std::endl;

        Point2D v1 = Point2D{ (m_a.first + m_b.first) / 2, (m_a.second + m_b.second) / 2 };
        Point2D v2 = Point2D{ (m_b.first + m_c.first) / 2, (m_b.second + m_c.second) / 2 };
        Point2D v3 = Point2D{ (m_c.first + m_d.first) / 2, (m_c.second + m_d.second) / 2 };
        Point2D v4 = Point2D{ (m_d.first + m_a.first) / 2, (m_d.second + m_a.second) / 2 };
        Point2D middle = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

        m_sw = new QuadTre(m_a, v1, middle, v4);
        m_sw->subDivide(n - 1);
        m_se = new QuadTre(v1, m_b, v2, middle);
        m_se->subDivide(n - 1);
        m_ne = new QuadTre(middle, v2, m_c, v3);
        m_ne->subDivide(n - 1);
        m_nw = new QuadTre(v4, middle, v3, m_d);
        m_nw->subDivide(n - 1);
    }
}

void QuadTre::print()
{
    //Point2D c = Point2D((m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2);

   
    //std::cout << "Center: " << std::to_string(c.first) << ", " << std::to_string(c.second) << std::endl;

    //for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
    //{
    //    Point2D xy = (*it)->getPosition2D();
    //    std::cout << (*it)->getName() << " : (" << xy.first << ", " << xy.second << ")" << std::endl;
    //    
    //}


}

void QuadTre::print_all()
{ 
    print();

    if (m_ne) m_ne->print_all();
    if (m_nw) m_nw->print_all();
    if (m_se) m_se->print_all();
    if (m_sw) m_sw->print_all();

}

void QuadTre::collisionCheck(glm::vec2 pos, std::string name)
{
    auto subtre = find(pos);
    int nameSize = name.size();
    for (auto it = subtre->begin(); it != subtre->end(); it++)
    {

        float x = abs(abs((*it)->getPosition2D().x) - abs(pos.x));
        float y = abs(abs((*it)->getPosition2D().y) - abs(pos.y));
        float lengthIT = sqrt(pow(x, 2) + pow(y, 2));

        if (lengthIT < 1.f)
        {
            (*it)->activate();
        }

        

        
    }

    subtre->print();
}

void QuadTre::removeEmpty()
{
    //um wtf

    //if (m_nw)
    //    m_nw->removeEmpty();

    //if (m_ne)
    //    m_ne->removeEmpty();

    //if (m_sw)
    //    m_sw->removeEmpty();

    //if (m_se)
    //    m_se->removeEmpty();

    //if (isLeaf() && m_Objects.size() == 0)
    //    delete this;
}

QuadTre* QuadTre::insert(VisualObject* gameObject)
{
    if (isLeaf())
    {
        m_Objects.push_back(gameObject);
        return this;
    }
    else {
        glm::vec2 m = glm::vec2{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };
        glm::vec2 g = gameObject->getPosition2D();

        if (g.y < m.y)
        {
            if (g.x < m.x)
                m_sw->insert(gameObject);
            else
                m_se->insert(gameObject);
        }
        else 
        {
            if (g.x < m.x)
                m_nw->insert(gameObject);
            else
                m_ne->insert(gameObject);
        }
    }
}

QuadTre* QuadTre::insert(VisualObject* gameObject, CollisionShape* shape)
{
    if (isLeaf())
    {
        // Makes sure subtre has no duplicate objects
        for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
            if ((*it) == gameObject)
                return this;

        m_Objects.push_back(gameObject);
        return this;
    }
    else
    {
        if (shape->overlap(m_sw->mShape))
        {
            m_sw->insert(gameObject, shape);
        }

        if (shape->overlap(m_se->mShape))
        {
            m_se->insert(gameObject, shape);
        }

        if (shape->overlap(m_ne->mShape))
        {
            m_ne->insert(gameObject, shape);
        }

        if (shape->overlap(m_nw->mShape))
        {
            m_nw->insert(gameObject, shape);
        }


        return nullptr;
    }
}


QuadTre* QuadTre::find(const glm::vec2 p)
{
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

        if (p.y < m.second)
        {
            if (p.x < m.first)
                m_sw->find(p);

            else
                m_se->find(p);
        }
        else
        {
            if (p.x < m.first)
                m_nw->find(p);
            else
                m_ne->find(p);
        }
    }
}

VisualObject* QuadTre::find(const std::string name)
{
    if (isLeaf() && m_Objects.size() > 0)
    {
        for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            if ((*it)->getName() == name)
                return *it;
        }
    }
    else if (m_ne)
    {
        m_ne->find(name);
    }
    else if (m_nw)
    {
        m_nw->find(name);
    }
    else if (m_se)
    {
        m_se->find(name);
    }
    else if (m_sw)
    {
        m_sw->find(name);
    }
}
