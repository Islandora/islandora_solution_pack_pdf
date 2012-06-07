<?php

/**
 *
 * @global type $base_url
 * @param array $variables
 *   an array of variables that will be passed to the theme function
 */
function islandora_pdf_preprocess_islandora_pdf(&$variables) {
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $islandora_object = $variables['islandora_object'];
  $repository = $islandora_object->repository;
  module_load_include('inc', 'islandora', 'includes/islandora_dublin_core');
  module_load_include('inc', 'islandora', 'includes/datastream');

  try {
    $dc = $islandora_object['DC']->content;
    $dc_object = Dublin_Core::import_from_xml_string($dc);
  } catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->as_formatted_array() : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_basic_image__' . str_replace(':', '_', $islandora_object->id);
  $variables['parent_collections'] = islandora_datastream_get_parents($islandora_object);
  global $base_url;
  if (isset($islandora_object['OBJ'])) {
    $full_size_url = $base_url . '/islandora/object/' . $islandora_object->id . '/datastream/OBJ/view';
    $variables['islandora_full_url'] = $full_size_url;
  }
  if (isset($islandora_object['TN'])) {
    $thumbnail_size_url = $base_url . '/islandora/object/' . $islandora_object->id . '/datastream/TN/view';
    $variables['islandora_thumbnail_img'] = '<img src="' . $thumbnail_size_url . '"/>';
  }
  if (isset($islandora_object['PREVIEW'])) {
    $preview_url = $base_url . '/islandora/object/' . $islandora_object->id . '/datastream/PREVIEW/view';
    $variables['islandora_preview_img'] = '<img src="' . $preview_url . '"/>';
    $variables['islandora_preview_link'] = l($variables['islandora_preview_img'], $variables['islandora_full_url'], array('html' => TRUE));
  }
}