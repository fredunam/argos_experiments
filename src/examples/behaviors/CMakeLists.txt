if(ARGOS_BUILD_FOR_SIMULATOR)
  include_directories(${CMAKE_SOURCE_DIR}/plugins/robots/kilobot/control_interface)

  #
  # Lab0: Blinky
  #
  add_executable(blinky blinky.c)
  target_link_libraries(blinky argos3plugin_simulator_kilolib)

  #
  # Lab1.2: Simple Movement
  #
  add_executable(simple_movement simple_movement.c)
  target_link_libraries(simple_movement argos3plugin_simulator_kilolib)

  #
  # Lab1.3: Non-blocked Movement
  #
  add_executable(nonblocked_movement nonblocked_movement.c)
  target_link_libraries(nonblocked_movement argos3plugin_simulator_kilolib)

  #
  # Lab2.1-2.2: Test Speaker and Test Listener
  #
  add_executable(test_speaker test_speaker.c)
  target_link_libraries(test_speaker argos3plugin_simulator_kilolib)
  add_executable(test_listener test_listener.c)
  target_link_libraries(test_listener argos3plugin_simulator_kilolib)

  #
  # Lab2.3-2.4: Modified Test Speaker and Test Listener
  #
  add_executable(test_speaker_mod test_speaker_mod.c)
  target_link_libraries(test_speaker_mod argos3plugin_simulator_kilolib)
  add_executable(test_listener_mod test_listener_mod.c)
  target_link_libraries(test_listener_mod argos3plugin_simulator_kilolib)

  #
  # Lab3: Disperse
  #
  add_executable(disperse disperse.c)
  target_link_libraries(disperse argos3plugin_simulator_kilolib)

  #
  # Lab4: Orbit
  #
  add_executable(orbit_star orbit_star.c)
  target_link_libraries(orbit_star argos3plugin_simulator_kilolib)
  add_executable(orbit_planet orbit_planet.c)
  target_link_libraries(orbit_planet argos3plugin_simulator_kilolib)

  #
  # Lab5: Move to Light
  #
  add_executable(move_to_light move_to_light.c)
  target_link_libraries(move_to_light argos3plugin_simulator_kilolib)

  #
  # Lab6: Simple Gradient
  #
  add_executable(gradient_simple gradient_simple.c)
  target_link_libraries(gradient_simple argos3plugin_simulator_kilolib)

  #
  # Lab7: Sync
  #
  add_executable(sync sync.c)
  target_link_libraries(sync argos3plugin_simulator_kilolib)

  #
  # Debugging example
  #
  add_executable(test_debug test_debug.h test_debug.c)
  target_link_libraries(test_debug argos3plugin_simulator_kilolib)

  #
  # shape formation : argos3 -c src/examples/experiments/kilobot_shape_formation.argos
  #
  add_executable(shape_formation_star shape_formation_star.c)
  target_link_libraries(shape_formation_star argos3plugin_simulator_kilolib)
  add_executable(shape_formation_planet shape_formation_planet.c)
  target_link_libraries(shape_formation_planet argos3plugin_simulator_kilolib)

  #
  # formation : argos3 -c src/examples/experiments/kilobot_formation.argos
  #
  add_executable(formation_star formation_star.c)
  target_link_libraries(formation_star argos3plugin_simulator_kilolib)
  add_executable(formation_planet formation_planet.c)
  target_link_libraries(formation_planet argos3plugin_simulator_kilolib)

  #
  # shape : argos3 -c src/examples/experiments/shape.argos
  #
  add_executable(shape shape.c)
  target_link_libraries(shape argos3plugin_simulator_kilolib)

  #
  # shape : argos3 -c src/examples/experiments/shape.argos
  #
  add_executable(orbit_star_2 orbit_star_2.c)
  target_link_libraries(orbit_star_2 argos3plugin_simulator_kilolib)
  add_executable(orbit_planet_2 orbit_planet_2.c)
  target_link_libraries(orbit_planet_2 argos3plugin_simulator_kilolib)

  add_executable(orbit_planet_3 orbit_planet_3.c)
  target_link_libraries(orbit_planet_3 argos3plugin_simulator_kilolib)


  #
  # shape : argos3 -c src/examples/experiments/entre.argos
  #
  add_executable(entre_planet_1 entre_planet_1.c)
  target_link_libraries(entre_planet_1 argos3plugin_simulator_kilolib)
  add_executable(entre_star_1 entre_star_1.c)
  target_link_libraries(entre_star_1 argos3plugin_simulator_kilolib)

  #
  # shape : argos3 -c src/examples/experiments/experiment_1.argos
  #
  add_executable(experiment_1 experiment_1.c)
  target_link_libraries(experiment_1 argos3plugin_simulator_kilolib)

  #
  # shape : argos3 -c src/examples/experiments/experiment_2.argos
  #
  add_executable(experiment_2 experiment_2.c)
  target_link_libraries(experiment_2 argos3plugin_simulator_kilolib)

  #
  # shape : argos3 -c src/examples/experiments/orbit_experiment.argos
  #
  add_executable(orbit_experiment orbit_experiment.c)
  target_link_libraries(orbit_experiment argos3plugin_simulator_kilolib)

  #
  # orbit double : argos3 -c src/examples/experiments/orbit_double.argos
  #
  add_executable(orbit_double orbit_double.c)
  target_link_libraries(orbit_double argos3plugin_simulator_kilolib)

  #
  # Triangle shape : argos3 -c src/examples/experiments/triangle.argos
  #
  add_executable(triangle_planet triangle_planet.c)
  target_link_libraries(triangle_planet argos3plugin_simulator_kilolib)
  add_executable(triangle_star triangle_star.c)
  target_link_libraries(triangle_star argos3plugin_simulator_kilolib)

endif(ARGOS_BUILD_FOR_SIMULATOR)
