<?php
/**
 * @file image.process.inc
 *
 * This file contains all the functions for image manipulation used in the
 * basic image solution pack.
 */

/**
 * creates the derivatives for this content model type
 * @param object $object
 *   tuque FedoraObject
 * @return string 
 */
function islandora_pdf_create_all_derivatives($object) {
  module_load_include('inc', 'islandora', 'includes/mime.detect');

  $mime_class = new MimeDetect();
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)),'error');
    return "";
  }

  $ext = $mime_class->getExtension($object['OBJ']->mimeType);

  $file_name = str_replace(':', '-', $object->id);
  $original_file = file_save_data($object['OBJ']->content, 'temporary://' . $file_name . 'OBJ.' . $ext);

  islandora_pdf_create_derivative($original_file, $object, 'TN', variable_get('islandora_pdf_thumbnail_width', 200), variable_get('islandora_pdf_thumbnail_height', 200));
  islandora_pdf_create_derivative($original_file, $object, 'PREVIEW',  variable_get('islandora_pdf_preview_width', 500), variable_get('islandora_pdf_preview_height', 700));

  file_delete($original_file);
}


/**
 *
 * @param object $file 
 *   stdclass
 */
function islandora_pdf_create_derivative($original, $object, $dsid, $width, $height) {

  $source = drupal_realpath($original->uri) . '[0]';
  $temp = drupal_tempnam('temporary://', $dsid);
  $dest = drupal_realpath($temp);

  $args['quality'] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
  $args['previewsize'] = '-resize ' . escapeshellarg("{$width}x{$height}");

  $context = array(
    'source' => $source,
    'destination' => $dest,
  );

  drupal_alter('imagemagick_arguments', $args, $context);
  // To make use of ImageMagick 6's parenthetical command grouping we need to make
  // the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline
  $command = escapeshellarg($source) . ' ' . implode(' ', $args) . ' ' . escapeshellarg("jpg:$dest");

  if (_imagemagick_convert_exec($command, $output, $error) !== TRUE) {
    return FALSE;
  }
  
  islandora_pdf_add_derivative($object, $dsid, $dest);
  file_unmanaged_delete($dest);
}

function islandora_pdf_add_derivative($object, $dsid, $dest) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = 'image/jpeg';
    $ds->setContentFromFile($dest);
    $object->ingestDatastream($ds);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error', FALSE);
  }
}
