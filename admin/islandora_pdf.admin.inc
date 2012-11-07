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
    '#title' => t('Thumbnail'),
    '#description' => t('Settings for PDF thumbnails'),
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The width of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_thumbnail_width', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The height of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_thumbnail_height', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview image'),
    '#description' => t('Settings for PDF preview image'),
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Max width'),
    '#description' => t('The maximum width of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_preview_width', 500),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#description' => t('The maximum height of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_preview_height', 700),
    '#size' => 5,
  );

  return system_settings_form($form);
}

/**
 * Admin form validate function
 */
function islandora_pdf_admin_validate($form, &$form_state) {

}