<?php

/**
 * @file
 * This file contains all admin functions.
 */

/**
 * Admin form function.
 */
function islandora_pdf_admin($form, &$form_state) {

  if (isset($form_state['values']['islandora_pdf_path_to_pdftotext'])) {
    $islandora_pdf_path_to_pdftotext = $form_state['values']['islandora_pdf_path_to_pdftotext'];
  }
  else {
    $islandora_pdf_path_to_pdftotext = variable_get('islandora_pdf_path_to_pdftotext', '/usr/bin/pdftotext');
  }
  exec($islandora_pdf_path_to_pdftotext, $output, $return_value);
  $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
      . t('pdftotext executable found at !url', array('!url' => "<strong>$islandora_pdf_path_to_pdftotext</strong>"));
  if ($return_value != 99) {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
        . t('Unable to find pdftotext executable at !url', array('!url' => "<strong>$islandora_pdf_path_to_pdftotext</strong>"));
  }

  $form = array();

  // AJAX wrapper for url checking.
  $form['islandora_pdf_url_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('TEXT'),
  );

  $form['islandora_pdf_url_fieldset']['islandora_pdf_allow_text_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to upload .txt files with PDFs"),
    '#description' => t("Uploaded text files are appended to PDFs as FULL_TEXT datastreams and are indexed into Solr."),
    '#default_value' => variable_get('islandora_pdf_allow_text_upload', FALSE),
  );

  $form['islandora_pdf_url_fieldset']['islandora_pdf_create_fulltext'] = array(
    '#type' => 'checkbox',
    '#title' => t("Extract text streams from PDFs using pdftotext"),
    '#description' => t("Extracted text streams are appended to PDFs as FULL_TEXT datastreams and are indexed into Solr. Uploading a text file takes priority over text stream extraction.
                           </br><strong>Note:</strong> PDFs that contain visible text do not necessarily contain text streams (e.g. images scanned and saved as PDFs). Consider converting text-filled images with no text streams to TIFFs and using the !book with !ocr enabled.", array(
                             '!book' => l(t('Book Solution Pack'), 'https://wiki.duraspace.org/display/ISLANDORA711/Book+Solution+Pack'),
                             '!ocr' => l(t('OCR'), 'https://wiki.duraspace.org/display/ISLANDORA711/Islandora+OCR'),
                           )),
    '#default_value' => variable_get('islandora_pdf_create_fulltext', FALSE),
  );

  $form['islandora_pdf_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );
  $form['islandora_pdf_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );

  $form['islandora_pdf_url_fieldset']['wrapper']['islandora_pdf_path_to_pdftotext'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdftotext executable'),
    '#default_value' => $islandora_pdf_path_to_pdftotext,
    '#description' => t('!confirmation_message',
        array(
          '!confirmation_message' => $confirmation_message)
    ),
    '#ajax' => array(
      'callback' => 'islandora_update_pdftotext_url_div',
      'wrapper' => 'islandora-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_pdf_create_fulltext"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['islandora_pdf_thumbnail_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnail'),
    '#description' => t('Settings for creating PDF thumbnail derivatives'),
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
    '#description' => t('Settings for creating PDF preview image derivatives'),
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

  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_pdf_viewers', 'application/pdf');
  return system_settings_form($form);
}

/**
 * Admin form validate function.
 */
function islandora_pdf_admin_validate($form, &$form_state) {
  if ($form_state['values']['islandora_pdf_create_fulltext']) {
    $islandora_pdf_path_to_pdftotext = $form_state['values']['islandora_pdf_path_to_pdftotext'];
    exec($islandora_pdf_path_to_pdftotext, $output, $return_value);
    if ($return_value != 99) {
      form_set_error('', t('Cannot extract text from PDF without a valid path to pdftotext.'));
    }
  }
}

/**
 * Function updates PDF to text URL div.
 */
function islandora_update_pdftotext_url_div($form, $form_state) {
  unset($form_state['submit_handlers']);
  $form_state['rebuild'] = TRUE;

  return $form['islandora_pdf_url_fieldset']['wrapper'];
}
