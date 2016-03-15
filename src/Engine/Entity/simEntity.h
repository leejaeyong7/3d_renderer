/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEntity.h
 * @version:
 * @summary:
 *      Declaration file for simulation objects
 *
 *============================================================================*/
#ifndef _SIM_ENTITY_H_
#define _SIM_ENTITY_H_
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <string>
#include <vector>
#include <cmath>
#include "simEntityOption.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                                 NAMESPACES                                 //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                               END NAMESPACES                               //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                            TYPEDEF DEFINITIONS                             //
//----------------------------------------------------------------------------//
typedef struct Position{
    double X;
    double Y;
    double Z;
} Position;

typedef struct Rotation{
    double Roll;
    double Pitch;
    double Yaw;
} Rotation;

/* typedef struct KeyPoint{ */
/*     double x; */
/*     double y; */
/* } KeyPoint; */

typedef struct Point{
    double x;
    double y;
    double z;
} Point;

typedef struct Triangle{
    Point a;
    Point b;
    Point c;
} Triangle;

typedef struct Rectangle{
    Triangle u;
    Triangle d;
} Rectangle;
//----------------------------------------------------------------------------//
//                          END TYPEDEF DEFINITIONS                           //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                              CLASS DEFINITION                              //
//----------------------------------------------------------------------------//
class SimEntity
{
public:
    /* Default constructor */
    SimEntity(std::string _name,
              double x, double y, double z,
              double a, double b, double c,
              std::string _meshPath = "");

    /* copy constructor */
    SimEntity(const SimEntity & obj);

    /* assignment operator */
    SimEntity& operator= (const SimEntity & rhs);

    /* default desructor. needs overloading */
    ~SimEntity();

//----------------------------------------------------------------------------//
//                                  SETTERS                                   //
//----------------------------------------------------------------------------//
    void setPosition(double x, double y, double z);

    void setRotation(double a, double b, double c);

    void setName(std::string new_name);

    void setMeshPath(std::string new_path);

//----------------------------------------------------------------------------//
//                                  GETTERS                                   //
//----------------------------------------------------------------------------//
    /**
     * gets position of entity
     * @param none
     * @return array size of 3 of double representing positional vector
     */
    const Position getPosition() const {return translation;};

    /**
     * gets position of entity
     * @param none
     * @return vector size of 3 of double representing rotational matrix
     */
    const Rotation getRotation() const {return rotation;};

    /**
     * gets name of this entity
     */
    const std::string getName() const {return name;};

    /**
     * gets name of this entity
     */
    const std::string getMeshPath() const {return meshPath;};

    /**
     * gets advanced option pointer
     */
    vector<AdvancedOption*>* getAdvancedOption() {return &advancedOption;};

    /**
     * gets keypoint vector pointer
     */
    vector<Point>* getKeyPoints() {return &keyPoints;};

    /**
     * gets Point vector pointer
     */
    vector<Point>* getPoints() {return &points;};

    /**
     * gets Triangle vector pointer
     */
    vector<Triangle>* getTriangles() {return &triangles;};

    /**
     * gets Rectangle vector pointer
     */
    vector<Rectangle>* getRectangles() {return &rectangles;};
//----------------------------------------------------------------------------//
//                                 CALLBACKS                                  //
//----------------------------------------------------------------------------//
    /**
     * callback called when edit is done
     */
    /* virtual void editCallback() = 0; */
    virtual void update() = 0;

    /**
     * Callback function when removeEntity is called
     * This is pure virtual function that must be overwritten
     */
    /* virtual void removeCallback() = 0; */




private:
    // translation coordinates
    Position translation;

    // rotation coordinates
    Rotation rotation;

    // name of entity
    std::string name;

    // path of mesh file
    std::string meshPath;

    // Advanced options vector
    vector<AdvancedOption*> advancedOption;

    // Vector of keypoints for each entity
    vector<Point> keyPoints;

    // Vector of Points for each entities
    vector<Point> points;

    // Vector of Triangles for each entity
    vector<Triangle> triangles;

    // Vector of Rectangles for each entity
    vector<Rectangle> rectangles;

    // check advanced option label
    struct checkLabel
    {
        checkLabel(std::string label) : str_holder(label) {}
        std::string str_holder;
        bool operator()(AdvancedOption* obj)
            {
                return obj->label == str_holder;
            }
    };
};
#endif
