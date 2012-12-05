<?php

/**
 * @file
 *
 * Contains all the functions required for adding derived datastreams from the uploaded file for the PDF content model.
 */

/**
 * Creates all the derivatives for this content model type
 *
 * @param FedoraObject $object
 *   The object to generate/add the deriviative datastreams to.
 *
 * @return boolean
 *   TRUE if all derivatives were created successfully, FALSE otherwise.
 */
function islandora_pdf_create_all_derivatives(FedoraObject $object) {
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)), 'error');
    return FALSE;
  }
  $success = TRUE;
  $file_uri = islandora_pdf_get_derivative_source_as_unmanaged_file($object);
  $success &= islandora_pdf_add_tn_derivative($object, $file_uri);
  $success &= islandora_pdf_add_preview_derivative($object, $file_uri);
  file_unmanaged_delete($file_uri);
  return $success;
}

/**
 * Gets the source of all derivatives as an unmanaged file, deletion of the file is left to the caller.
 *
 * @param FedoraObject $object
 *   The object to generate/add the deriviative datastreams to.
 *
 * @return string
 *   The file path to the unmanaged derivative source file.
 */
function islandora_pdf_get_derivative_source_as_unmanaged_file(FedoraObject $object) {
  $mime_detect = new MimeDetect();
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);
  $file_uri = file_create_filename("{$file_name}.OBJ.{$ext}", 'temporary://');
  $object['OBJ']->getContent($file_uri);
  return $file_uri;
}

/**
 * Creates a JPG deriviative for the TN datastream.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the pdf file the derivative will be generated from.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_pdf_add_tn_derivative(FedoraObject $object, $file_uri) {
  $width = variable_get('islandora_pdf_thumbnail_width', 200);
  $height = variable_get('islandora_pdf_thumbnail_height', 200);
  return islandora_pdf_add_jpg_derivative($object, $file_uri, 'TN', $width, $height);
}

/**
 * Creates a JPG deriviative for the PREVIEW datastream.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the pdf file the derivative will be generated from.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_pdf_add_preview_derivative(FedoraObject $object, $file_uri) {
  $width = variable_get('islandora_pdf_preview_width', 500);
  $height = variable_get('islandora_pdf_preview_height', 700);
  return islandora_pdf_add_jpg_derivative($object, $file_uri, 'PREVIEW', $width, $height);
}

/**
 * Creates a JPG deriviative of the given file.
 *
 * @param FedoraObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the pdf file the derivative will be generated from.
 * @param string $dsid
 *   The datastream ID to use for the derived datastream.
 * @param integer $width
 *   The width to make the derived datastream.
 * @param integer $height
 *   The height to make the derived datastream.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_pdf_add_jpg_derivative(FedoraObject $object, $file_uri, $dsid, $width, $height) {
  $success = FALSE;
  $derivative_file_uri = islandora_pdf_create_jpg_derivative($file_uri, $dsid, $width, $height);
  if ($derivative_file_uri !== FALSE) {
    $success = islandora_pdf_add_datastream($object, $dsid, $derivative_file_uri);
    file_unmanaged_delete($derivative_file_uri);
  }
  if ($success == FALSE) {
    drupal_set_message(t('Failed to create @dsid derivative.', array('@dsid' => $dsid)), 'error');
  }
  return $success;
}

/**
 * Creates a jpg derivative from the given pdf file.
 *
 * @param string $file_uri
 *   The URI to the pdf file the derivative will be generated from.
 * @param string $dsid
 *   Used to identify the generated file.
 * @param integer $width
 *   The width to make the derived datastream.
 * @param integer $height
 *   The height to make the derived datastream.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_pdf_create_jpg_derivative($file_uri, $dsid,  $width, $height) {
  $source = drupal_realpath($file_uri) . '[0]';
  $matches = array();
  preg_match("/\/([^.]*).*$/", $source, $matches); // Get the base name of the source file.
  $temp = drupal_tempnam("temporary://", "{$matches[1]}.$dsid.jpg");
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
  $output = '';
  $ret = -1;
  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
    $message = 'imagemagick convert failed to create derivative<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
    $variables = array('@ret' => $ret, '@command' => $command, '@output' => $output);
    watchdog('islandora_pdf', $message, $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

/**
 * Adds the given file as a datastream to the given object using the given datastream id to identify it.
 *
 * @param FedoraObject $object
 *   The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 */
function islandora_pdf_add_datastream($object, $datastream_id, $file_uri) {
  try {
    $mime_detector = new MimeDetect();
    $ds = $object->constructDatastream($datastream_id, "M");
    $ds->label = $datastream_id;
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri), FALSE);
    $object->ingestDatastream($ds);
    return TRUE;
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
    return FALSE;
  }
}
