<?php

/**
 * @file
 * This is the template file for the pdf object
 *
 * @TODO: Add documentation about this file and the available variables
 * @TODO: don't call drupal_set_title here
 */
?>

<div class="islandora-pdf-object islandora">
  <div class="islandora-pdf-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-pdf-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
  <div class="islandora-pdf-sidebar">
    <?php if (!empty($dc_array['dc:description']['value'])): ?>
      <h2><?php print $dc_array['dc:description']['label']; ?></h2>
      <p><?php print $dc_array['dc:description']['value']; ?></p>
    <?php endif; ?>
    <?php if($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $key => $value): ?>
            <li><?php print $value['label_link'] ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
  </div>
  </div>
  <fieldset class="collapsible collapsed islandora-pdf-metadata">
  <legend><span class="fieldset-legend"><?php print t('Extended details'); ?></span></legend>
    <div class="fieldset-wrapper">
      <dl class="islandora-inline-metadata islandora-pdf-fields">
        <?php $row_field = 0; ?>
        <?php foreach ($dc_array as $key => $value): ?>
          <dt class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">
            <?php print $value['label']; ?>
          </dt>
          <dd class="<?php print $value['class']; ?><?php print $row_field == 0 ? ' first' : ''; ?>">
            <?php print $value['value']; ?>
          </dd>
          <?php $row_field++; ?>
        <?php endforeach; ?>
      </dl>
    </div>
  </fieldset>
</div>