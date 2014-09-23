<?php
/**
 * @file
 * Template for the weather module.
 */
?>
<div class="weather">
  <div class="place">
    <?php print $weather->real_name; ?>
    <?php if (isset($weather->reported_on)): ?>
    , <span class="date-reported">
      <?php print $weather->reported_on; ?>
    </span>
    <?php endif ?>
  </div>
  <div class="image">
    <?php print $weather->image; ?> <span class="condition"><?php print $weather->condition; ?></span>
  </div>
  <?php if (isset($weather->temperature)): ?>
    <?php if (isset($weather->windchill)): ?>
  <div class="temperature"><?php print t("Temperature: !temperature1, feels like !temperature2", array(
    '!temperature1' => $weather->temperature,
    '!temperature2' => $weather->windchill
  )); ?></div>
  <?php else: ?>
  <div class="temperature"><?php print t("Temperature: !temperature",
    array('!temperature' => $weather->temperature)); ?></div>
  <?php endif ?>
  <?php endif ?>
  <?php if (isset($weather->wind)): ?>
  <div class="wind"><?php print t('Wind: !wind',
        array('!wind' => $weather->wind)); ?></div>
  <?php endif ?>
  <?php if (isset($weather->rel_humidity)): ?>
  <div class="humidity"><?php print t('Rel. Humidity: !rel_humidity',
        array('!rel_humidity' => $weather->rel_humidity)); ?></div>
  <?php endif ?>
  
  <?php if (isset($weather->station)): ?>
    <small>
      <?php print t('Location of this weather station:'); ?><br />
      <?php print $weather->station; ?>
    </small>
    <br />
  <?php endif ?>
</div>
