/*============================================================================
 * @author: Jae Yong Lee
 * @file: simEntity.h
 * @version:  
 * @summary:
 *      Declaration file for simulation objects
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                INCLUDE GUARDS
//----------------------------------------------------------------------------//
#ifndef _SIM_ENTITY_H_
#define _SIM_ENTITY_H_

//----------------------------------------------------------------------------//
//                                   Includes
//----------------------------------------------------------------------------//
#include <string>
#include <vector>
#include "simEntityOption.h"

//----------------------------------------------------------------------------//
//                                  Namespaces
//----------------------------------------------------------------------------//
using namespace std;

//----------------------------------------------------------------------------//
//                                Global Variables
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

//----------------------------------------------------------------------------//
//                               Class Declaration
//----------------------------------------------------------------------------//

class SimEntity
{
    public:
        /**
         * Constructor with position/rotation and empty scenenode
         * @param double x - x axis coordinate
         * @param double y - y axis coordinate
         * @param double z - z axis coordinate
         * @param double a - a axis coordinate
         * @param double b - b axis coordinate
         * @param double c - c axis coordinate
         * @param std::stringw p_name - name of entity
         * @return SimEntity object with no mesh to render
         */
        SimEntity(std::string _name,
                double x, double y, double z,
                double a, double b, double c);

        /**
         * Constructor with position/rotation and empty scenenode
         * @param std::string _name - name of entity
         * @param double x - x axis coordinate
         * @param double y - y axis coordinate
         * @param double z - z axis coordinate
         * @param double a - a axis coordinate
         * @param double b - b axis coordinate
         * @param double c - c axis coordinate
         * @param std::string _meshPath - path of mesh file
         * @return SimEntity object with no mesh to render
         */
        SimEntity(std::string _name,
                double x, double y, double z,
                double a, double b, double c,
                std::string _meshPath);

        /**
         * Copy Constructor
         * @param SimEntity - SimEntity Object to copy from
         * @return Copied SimEntity object
         */
        SimEntity(const SimEntity & obj);

        /**
         * Assignment Operator
         * @param SimEntity - Simentity Object to copy from
         * @return Copied SimEntity object
         */
        SimEntity& operator= (const SimEntity & rhs);

        /**
         * Destructor
         * Initialize Irrlicht, Entities, and physics
         */
        virtual ~SimEntity();


        /**
         * Callback function when removeEntity is called 
         * This is pure virtual function that must be overwritten
         */
        virtual void removeCallback() = 0;

        /**
         * sets position of entity
         * @param double x - x axis coordinate
         * @param double y - y axis coordinate
         * @param double z - z axis coordinate
         * @return none
         */
        void setPosition(double x, double y, double z);

        /**
         * sets rotation of entity
         * @param double a - a axis coordinate
         * @param double b - b axis coordinate
         * @param double c - c axis coordinate
         * @return none
         */
        void setRotation(double a, double b, double c);

        /**
         * sets name of entity
         * @return none
         */
        void setName(std::string new_name);

        /**
         * sets path name of entity
         * @return none
         */
        void setMeshPath(std::string new_path);


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
        vector<double>* getKeyPoints() {return &keyPoints;};

    private:
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
        vector<double> keyPoints;

};
#endif
