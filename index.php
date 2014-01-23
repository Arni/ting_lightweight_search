<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function load_inc() {
//$path = drupal_get_path('module', 'ting_ligtweight_search');
  $path = DRUPAL_ROOT . '/profiles/ding2/modules/ting_lightweight_search';
file_put_contents("/home/quickstart/work/debug/debuglight2.txt", print_r($path , TRUE), FILE_APPEND);
require $path . "/lib/request/TingClientRequestFactory.php";
require $path . "/lib/request/TingClientRequest.php";
require $path . "/lib/request/TingClientSearchRequest.php";
require $path . "/lib/TingClient.php";
require $path . "/lib/adapter/TingClientRequestAdapter.php";
require $path . "/lib/exception/TingClientException.php";
require $path . "/lib/result/object/TingClientObject.php";
require $path . "/lib/result/object/TingClientObjectCollection.php";
require $path . "/lib/result/object/data/TingClientObjectData.php";
require $path . "/lib/result/search/TingClientSearchResult.php";
require $path . "/lib/result/search/TingClientFacetResult.php";
require $path . "/lib/log/TingClientLogger.php"; 
require $path . "/lib/log/TingClientVoidLogger.php"; 
require $path . "/lib/nanosoap/nanosoap.inc"; 
require $path . "/includes/search.inc"; 
require $path . "/includes/object.inc"; 
require $path . "/includes/theme.inc"; 
require $path . "/includes/availability.inc";
require $path . "/includes/facets.inc";
require $path . "/includes/pager.inc";
}

function search ($query) {
  load_inc();
$output = '';
if ($query) {
  $search_results = ting_do_search($query);
  $results = parse_search_results($search_results);
  $facets = process_facets($search_results->facets, $query, '/search/ting/' . $query);
  $more_pages = ($search_results->more ? 1 : 0);
  $pager = make_pager(1, 1 + $more_pages, $query);
  $output = theme_results ($results, $facets, $pager);
  $availability = process_availability($results);
}

header('Content-Type: application/json');
$json_output = array("result_html" => $output, "availability" => $availability );
return $output;
}


  /**
   * Process series information
   * This could be handled more elegantly if we had better structured data.
   * For now we have to work with what we got to convert titles to links
   * Series information appear in the following formats:
   * - Samhørende: [title 1] ; [title 2] ; [title 3]
   * - [volumne number]. del af: [title]
   */
  function process_series_description($series) {
    $result = '';
    $parts = explode(':', $series);

    if (is_array($parts) && count($parts >= 2)) {
      $prefix = $parts[0] . ': ';

      if (stripos($prefix, 'del af:') !== FALSE) {
        $title = $parts[1];
        $result = $prefix . l($title, '/search/ting/' . '"' . $title . '"');
      } else if (stripos($prefix, 'Samhørende:') !== FALSE) {

        $titles = $parts[1];
        // Multiple titles are separated by ' ; '. Explode to iterate over them
        $titles = explode(' ; ', $titles);
        foreach ($titles as &$title) {
          $title = trim($title);
          // Some title elements are actually volumne numbers. Do not convert these to links
          if (!preg_match('/(nr.)? \d+/i', $title)) {
            $title = l($title, '/search/ting/' . '"' . $title . '"');
          }
        }
        // Reassemple titles
        $titles = implode(', ', $titles);
        $result = $prefix . ' ' . $titles;
      }
    }

    return $result;
  }



function parse_search_results($results) {
  $output = array();
  foreach ($results->collections as $collection) {    
    if (isset($collection->objects[0])) {
      $object = $collection->objects[0];
      $output[] = array(
       'title'    => getTitle($object),
       'creators' => getCreators($object),
       'subjects' => getSubjects($object),
       'abstract' => getAbstract($object),
       'local_id' => getLocalId($object),
       'owner_id' => getOwnerId($object),
       'date'     => getObjectDate($object),
       'serie_title' => getSerieTitle($object),
       'serie_description' => getSerieDescription($object),
       'types' => process_types ($collection),
      );
    } 

  }
  return $output;
}

function process_types($collection) {
  $types = array();
  foreach ($collection->objects as $object) {
    $type_value = getObjectType($object);
    $type_exists_allready = false;
    foreach ($types as &$type) {
      if (is_same_type ($type['type'], $type_value, getLocalId($object))) {
        $type_exists_allready = true;
        $type['count'] = $type['count'] + 1;
        if (is_check_availability_type(getAc_source($object), $type_value)) {
          $type['object_ids'][] = getLocalId($object);
        }
        break;
      }
    }
    if (!$type_exists_allready) {
      $new_type = array(
        'type' => $type_value,
        'local_id' => getLocalId($object),
        'owner_id' => getOwnerId($object),
        'count' => 1,
        'object_ids' => array(),
      );
      if (is_check_availability_type(getAc_source($object), $type_value)) {
        $new_type['object_ids'][] = getLocalId($object);
      }
      $types[] = $new_type;
    }
  }
  return $types;
}



?>
