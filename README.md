# PDF Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_pdf.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_pdf)

## Introduction

Loads all required Fedora Objects, and creates an empty collection object to accept PDFs.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [ImageMagick](http://www.imagemagick.org/script/index.php) - Debian/Ubuntu `sudo apt-get install imagemagick`
* [ImageMagick module](https://www.drupal.org/project/imagemagick)
    * ensure that the **full path** to Imagemagick's convert is specified in the Image Toolkit (admin/config/media/image-toolkit)
* [pdftotext](http://poppler.freedesktop.org) -  Debian/Ubuntu `sudo apt-get install poppler-utils`
* [ghostscript](http://www.ghostscript.com) - Debian/Ubuntu `sudo apt-get install ghostscript`

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration options are found at Administration » Islandora »  Solution pack configuration »  PDF Solution Pack (admin/islandora/solution_pack_config/pdf) and include:

* whether users are allowed to upload .txt files along with the PDFs, to be used for full text indexing
* whether text streams should be extracted automatically from the PDFs using pdftotext. If enabled, the path to `pdftotext` should be provided
* whether PDF/A derivatives should be created. If enabled, the path to `gs` (GhostScript) should be provided
* image sizes for thumbnail and preview derivatives, as well as color spaces that should be used for generating them
* whether the dUseCIEColor switch should be used for GhostScript. Not recommended for GhostScript versions 9.11 or higher
* which viewer should be used


![Configuration](https://user-images.githubusercontent.com/10065857/61782954-4361d580-ae07-11e9-83d2-77900f8f76fb.png)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/PDF+Solution+Pack).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jonathan Green](https://github.com/jonathangreen)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
