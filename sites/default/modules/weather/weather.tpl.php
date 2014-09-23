<?php
/**
 * @file
 * Template for the weather module.
 */
$i = 1;
?>
<div class="weather">
  <?php foreach($weather as $place): ?>
    <div class="place">
      <?php print strip_tags($place['link']); ?>, 
      <span class="date-reported">
        <?php print format_date(time(), 'clima', '', NULL, NULL);?>
      </span>
    </div>
    <?php foreach($place['forecasts'] as $forecast): ?>
      <?php foreach($forecast['time_ranges'] as $time_range => $data): ?>
      <div id="time_range-<?php print $i;?>">
        <div class="image">
          <?php print $data['symbol']; ?> <span class="condition"><?php print $data['condition']; ?></span>
        </div>
        <div class="temperature">
          Temperatura: <span style="white-space:nowrap;"><?php print $data['temperature']; ?></span>
        </div>
      </div>
      <?php $i++; break;?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
</div>
