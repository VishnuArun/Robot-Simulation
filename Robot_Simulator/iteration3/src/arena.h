/**
 * @file arena.h
 *
 * @copyright 2017 3081 Staff,  All rights reserved.
 *
 */

#ifndef SRC_ARENA_H_
#define SRC_ARENA_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include <iostream>
#include <vector>

#include "src/common.h"
#include "src/food.h"
#include "src/entity_factory.h"
#include "src/robot.h"
#include "src/communication.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
struct arena_params;

/**
 * @brief The main class for the simulation of a 2D world with many entities
 * running around.
 *
 * While GraphicsArenaViewer handles the graphics, Arena handles all the
 * data and all the entities (it is the model of MVC). It manages the
 * interaction among the entities in the Arena.
 *
 */

class Arena {
 public:
  /**
   * @brief Arena's constructor.
   *
   * @param params A arena_params passed down from main.cc for the
   * initialization of Arena and the entities therein.
   *
   * Initialize all private variables and entities.
   */
  explicit Arena(const struct arena_params *const params);

  /**
   * @brief Arena's destructor. `delete` all entities created.
   */
  ~Arena();

  /**
   * @brief Advance the simulation by the specified # of steps.
   *
   * @param[in] dt The # of steps to increment by. This is
   * practically unused because the arena state is advanced
   * incrementally at a fixed rate.
   *
   * If `dt == 0`, `return` immediately. Otherwise calls
   * Arena::UpdateEntitiesTimestep() once.
   */
  void AdvanceTime(double dt);


  void AdjustLightSensitivity(double sense);
  void ParameterReset();
  void AddRobot(int quantity);
  void AddLight(int quantity);
  void AddEntity(EntityType type, int quantity);

  /**
   * @brief
   */
  void AcceptCommand(Communication com);

  /**
   * @brief Reset all entities in Arena.
   */
  void Reset();



  /**
   * @brief Under certain circumstance, the compiler requires that the
   * assignment operator is not defined. This `deletes` the default
   * assignment operator.
   */
  Arena &operator=(const Arena &other) = delete;

  /**
   * @brief Under certain circumstance, the compiler requires that the copy
   * constructor is not defined. This `deletes` the default copy constructor.
   */
  Arena(const Arena &other) = delete;

  /**
   * @brief Determine if two entities have collided in the Arena. Collision is
   * defined as the distance between two entities being less than the sum of
   * their radii.
   *
   * @param mobile_e This entity is definitely moving.
   * @param other_e This entity might be mobile or immobile.
   * @param[out] True if entities overlapping.
   *
   **/
  bool IsColliding(
    ArenaMobileEntity * const mobile_e, ArenaEntity * const other_e);

  /**
  * @brief Move the mobile entity to the edge of the other without overlap.
  * Without this, entities tend to get stuck inside one another.
  **/
  void AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
    ArenaEntity *const other_e);

  /**
   * @brief Determine if a particular entity has gone out of the boundaries of
   * the simulation (i.e. has collided with any one of the walls).
   *
   * @param ent The entity to check.
   * @param [out] An entity type signifying wall (e.g. kRightWall). kUndefined
   * if no collision.
   *
   * The checked entity's position will be updated to a "back-off position" so
   * that it won't get stuck into a wall. The calculation of the "back-off
   * position" is technically not accurate, but good enough for our purpose.
   */
  EntityType GetCollisionWall(ArenaMobileEntity * const ent);

  /**
  * @brief Move the entity to the edge of the wall without overlap.
  * Without this, entities tend to get stuck in walls.
  **/
  void AdjustWallOverlap(ArenaMobileEntity * const ent, EntityType wall);


  /**
   * @brief Update all entities for a single timestep.
   *
   * First calls each entity's TimestepUpdate method to update their speed,
   * heading angle, and position. Then check for collisions between entities
   * or between an entity and a wall.
   */
  void UpdateEntitiesTimestep();

  std::vector<class ArenaEntity *> get_entities() const { return entities_; }

  /**
   * @brief Get the Robot (there's only 1) in Arena.
   *
   * @return A pointer to the Robot.
   */
  std::vector<class Robot *>robot() const { return robot_; }

  double get_x_dim() { return x_dim_; }
  double get_y_dim() { return y_dim_; }

  int get_game_status() const { return game_status_; }
  void set_game_status(int status) { game_status_ = status; }

// GUI, number of robots get/set
  void set_robots(int n) {
    robot_quantity_ = n;
  }

  int get_robots() { return robot_quantity_;}

  // GUI, get/set number of lights

  void set_lights(int n) {
    light_quantity_ = n;
  }
  int get_lights() {return light_quantity_;}


  void set_light_sensitivty(double n) {
    light_sensitivity_ = n;
  }

  int get_food() {
    return food_quantity_;
  }

  void set_food(int n) {
    food_quantity_ = n;
  }

  bool get_food_button_status() {
    return food_button_active_;
  }

  void set_food_button_status(bool b) {
    food_button_active_ = b;
  }

  int get_robot_ratio() {
    return robot_ratio_;
  }

  void set_robot_ratio(int r) {
    robot_ratio_ = r;
  }

 private:
  // Dimensions of graphics window inside which entities must operate
  double x_dim_;
  double y_dim_;

  // Used to create all entities within the arena
  EntityFactory *factory_;

  int robot_ratio_ = 5;


  Light *light_{nullptr};

  // All entities mobile and immobile.
  std::vector<class ArenaEntity *> entities_;

  // A subset of the entities -- only those that can move.
  std::vector<class ArenaMobileEntity *> mobile_entities_;

  // A subset of entites who have type Robot
  std::vector<class Robot *> robot_;
  // win/lose/playing state
  int game_status_;

  // number of robots/lights to be created from GUI_MENU_GAI
  int robot_quantity_ = 5;

  int light_quantity_ = 5;

  int food_quantity_ = 5;

  bool food_button_active_ = false;


  double light_sensitivity_ = 1200.0;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_H_
