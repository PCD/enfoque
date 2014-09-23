<?php
/**
 * Increments the visits.
 */
function fakecounter_visit_increment() {
  $increment = rand(0, 1);
  $visits = variable_get('fakecounter-visits', 0);
  $visits += $increment;
  variable_set('fakecounter-visits', $visits);
  return true;
}