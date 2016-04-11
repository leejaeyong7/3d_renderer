/*============================================================================
 * @author: Jae Yong Lee
 * @file: enumerations.h
 * @version:  
 * @summary:
 *      Includes all enumerations used commonly throughout simulator
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                Include Guards
//----------------------------------------------------------------------------//
#ifndef _SIM_ENUM_H_
#define _SIM_ENUM_H_
#include <string>

//----------------------------------------------------------------------------//
//                                Enumerations
//----------------------------------------------------------------------------//
typedef enum optionType{
    STRING,
    DOUBLE,
    INTEGER,
} OptionType;

typedef enum entityType{
    ENTITY_TYPE_ROBOT,
    ENTITY_TYPE_SENSOR,
    ENTITY_TYPE_ENVIRONMENT
} EntityType;

// gui elements
enum
{
    QUIT_BUTTON = 0x10000,
    FEATURE_BUTTON,

    // used for add prompt
    PROMPT_ADD_WINDOW,
    PROMPT_EDIT_WINDOW,
    PROMPT_COMBO,
    PROMPT_NAME,
    PROMPT_ADVANCED_BOX,

    PROMPT_POS_X,
    PROMPT_POS_Y,
    PROMPT_POS_Z,
    PROMPT_ROT_A,
    PROMPT_ROT_B,
    PROMPT_ROT_C,
    PROMPT_ADV_INPUT,

    CLOSE_BUTTON,
    CONFIRM_BUTTON,
    APPLY_BUTTON,
    REMOVE_BUTTON,
    RUN_BUTTON,
    FPS_CAMERA_BUTTON,
    MAYA_CAMERA_BUTTON,
    PLACER_CAMERA_BUTTON,

    ADD_PLANE_BUTTON,
    ADD_CUBE_BUTTON,
    ADD_PYRAMID_BUTTON,

    // context menu
    ADD_ROBOT,
    ADD_SENSOR,
    ADD_ENVIRONMENT,

    EDIT_ROBOT,
    EDIT_SENSOR,
    EDIT_ENVIRONMENT,

    EDIT_PATH,

    ATTACH_ENTITY,
    DETACH_ENTITY,

    // used for attach window
    ATTACH_WINDOW,
    ATTACH_COMBO1,
    ATTACH_COMBO2,

    // used for path edit window
    PATH_WINDOW,
    PATH_COMBO,
    PATH_CAMERA_COMBO,
    PATH_FPS_SCROLL,
    PATH_FOLDER,
    PATH_FPS,
    PATH_REMOVE_BUTTON,

    PATH_POS_X,
    PATH_POS_Y,
    PATH_POS_Z,
    PATH_ROT_A,
    PATH_ROT_B,
    PATH_ROT_C,
    PATH_DUR,

    CAMERA_COMBO,
    CAPTURE_BUTTON
};

// subentities
enum
{
    SUB_ENTITY_ROBOT_QUAD,
    SUB_ENTITY_ROBOT_GROUND,
    SUB_ENTITY_SENSOR_CAM,
    SUB_ENTITY_ENVIRONMENT_PLANE,
    SUB_ENTITY_ENVIRONMENT_CUBE,
    SUB_ENTITY_ENVIRONMENT_SPHERE
};
// prompts
enum{
    ADD_ENTITY_PROMPT = 0x5000,
    EDIT_ENTITY_PROMPT,
    ATTACH_ENTITY_PROMPT,
    DETACH_ENTITY_PROMPT
};

#endif
