<?php

/**
 * @file
 * This file contains all admin functions
 */

/**
 * Admin form function
 */
function islandora_pdf_admin() {
  $form = array();

  $form['islandora_pdf_thumbnail_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Thumbnail',
    '#description' => 'Settings for PDF Thumbnails',
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#description' => 'The width of the thumbnail in pixels.',
    '#default_value' => variable_get('islandora_pdf_thumbnail_width', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_height'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#description' => 'The width of the thumbnail in pixels.',
    '#default_value' => variable_get('islandora_pdf_thumbnail_height', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Preview Image',
    '#description' => 'Settings for PDF Preview Image',
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#description' => 'The width of the preview in pixels.',
    '#default_value' => variable_get('islandora_pdf_preview_width', 500),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_height'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#description' => 'The width of the preview in pixels.',
    '#default_value' => variable_get('islandora_pdf_preview_height', 700),
    '#size' => 5,
  );

  return system_settings_form($form);
}

/**
 * Admin form validate function
 */
function islandora_pdf_admin_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['islandora_pdf_thumbnail_width'])) {
    form_set_error('islandora_pdf_thumbnail_width', t('Please enter a numeric value.'));
  }
  if (!is_numeric($form_state['values']['islandora_pdf_thumbnail_height'])) {
    form_set_error('islandora_pdf_thumbnail_height', t('Please enter a numeric value.'));
  }
  if (!is_numeric($form_state['values']['islandora_pdf_preview_width'])) {
    form_set_error('islandora_pdf_preview_width', t('Please enter a numeric value.'));
  }
  if (!is_numeric($form_state['values']['islandora_pdf_preview_height'])) {
    form_set_error('islandora_pdf_preview_height', t('Please enter a numeric value.'));
  }
}