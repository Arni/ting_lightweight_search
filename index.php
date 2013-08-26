<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

require "./lib/request/TingClientRequestFactory.php";
require "./lib/request/TingClientRequest.php";
require "./lib/request/TingClientSearchRequest.php";
require "./lib/TingClient.php";
require "./lib/adapter/TingClientRequestAdapter.php";
require "./lib/exception/TingClientException.php";
require "./lib/result/object/TingClientObject.php";
require "./lib/result/object/TingClientObjectCollection.php";
require "./lib/result/object/data/TingClientObjectData.php";
require "./lib/result/search/TingClientSearchResult.php";
require "./lib/log/TingClientLogger.php"; 
require "./lib/log/TingClientVoidLogger.php"; 
require "./lib/nanosoap/nanosoap.inc"; 

$query = $_REQUEST['q'];
$search_results = ting_do_search($query);
$results = parse_search_results($search_results);
$output = my_theme_search($results); 

echo $output;



function my_theme($template, $vars) {
  ob_start();
  extract($vars);
  require $template;
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

function my_theme_search($results) {
  $items =array();
  foreach ($results as $result) {
    $vars = array();
    $collection_link_part = '<a href="/ting/collection/' . $result['owner_id'] . ':' . $result['local_id'] . '">';
    if (isset($result['title'])) {
      $vars['title_link'] = $collection_link_part  . $result['title'] . '</>';
    }
    $image = '<img src="https://www.randersbib.dk/ting/covers/collection/80_x/' . $result['owner_id'] . ':' . $result['local_id'] . '">'; 
    $vars['image'] = $collection_link_part  . $image . '</>';
    //<a class="author" href="/search/ting/dc.creator%3DMaeve%20Binchy">Maeve Binchy</a>
    if (isset($result['creators']) && !empty($result['creators'])) {
              //file_put_contents("/home/quickstart/work/debug/debugperf3.txt", print_r($result['creators'] , TRUE), FILE_APPEND);
      $creators = 'Af ';
      foreach ($result['creators'] as $creator) {
        $creators .= '<a class="author" href="/search/ting/dc.creator=' . $creator . '"> ' . $creator . '</a> ';
      }
      $vars['creators'] = $creators;
    }
    $items[] = my_theme('./item-template.php', $vars);
  }
  $output = my_theme('./search_page_template.php', array('items' => $items));
  return $output;
}


function ting_do_search($query, $page = 1, $results_per_page = 10, $options = array()) {
  $url = array('search' => "opensearch.addi.dk/2.2/");
  $request_factory = new TingClientRequestFactory($url);
  $request = $request_factory->getSearchRequest();
  $request->setQuery($query);
  $request->setAgency("773000");

  $request->setStart($results_per_page * ($page - 1) + 1);
  $request->setNumResults($results_per_page);

//  if (!isset($options['facets']) and module_exists('ding_facetbrowser')) {
//    $options['facets'] = array();
//    // Populate facets with configured facets.
//    foreach (variable_get('ding_facetbrowser_facets', array()) as $facet) {
//      $options['facets'][] = $facet['name'];
//    }
//  }

  //$request->setFacets((isset($options['facets'])) ? $options['facets'] : array('facet.subject', 'facet.creator', 'facet.type', 'facet.category', 'facet.language', 'facet.date', 'facet.acSource'));
  //$request->setNumFacets((isset($options['numFacets'])) ? $options['numFacets'] : ((sizeof($request->getFacets()) == 0) ? 0 : 10));
  //if (isset($options['sort']) && $options['sort']) {
   // $request->setSort($options['sort']);
  //}
  if (isset($options['collectionType'])) {
    $request->setCollectionType($options['collectionType']);
  }
  $request->setAllObjects(isset($options['allObjects']) ? $options['allObjects'] : FALSE);

  $request->setProfile("opac");


  // Apply custom ranking if enabled.
//  if (variable_get('ting_ranking_custom', FALSE)) {
//    $fields = array();
//    foreach (variable_get('ting_ranking_fields', array()) as $field) {
//      $fields[] = array(
//        'fieldName' => $field['field_name'],
//        'fieldType' => $field['field_type'],
//        'weight' => $field['weight'],
//      );
//    }

//    if (!empty($fields)) {
//      // Add the default anyIndex boosts.
//      $fields[] = array(
//        'fieldName' => 'cql.anyIndexes',
//        'fieldType' => 'phrase',
//        'weight' => 1,
//      );
//
//      $fields[] = array(
//        'fieldName' => 'cql.anyIndexes',
//        'fieldType' => 'word',
//        'weight' => 1,
//      );
//
//      $request->userDefinedRanking = array('tieValue' => 0.1, 'rankField' => $fields);
//    }
 // }
  // Otherwise, use the ranking setting.
//  else {
//    $request->setRank((isset($options['rank']) && $options['rank']) ? $options['rank'] : 'rank_general');
//  }

  // Apply custom boosts if any.
//  $boosts = variable_get('ting_boost_fields', array());
//
//  if ($boosts) {
//    $uboosts = array();
//    foreach ($boosts as $boost_field) {
//      $uboosts[] = array(
//        'fieldName' => $boost_field['field_name'],
//        'fieldValue' => $boost_field['field_value'],
//        'weight' => $boost_field['weight'],
//      );
//    }
//    $request->userDefinedBoost = array('boostField' => $uboosts);
//  }

  $search_result = ting_execute($request);
  // Replace collections with proper TingCollection objects.
//  if ($search_result && is_array($search_result->collections)) {
//    $ids = array();
//    foreach ($search_result->collections as &$collection) {
//      if (isset($collection->objects[0])) {
//        $ids[] = $collection->objects[0]->id;
//      }
//    }
//    if (!isset($options['reply_only']) || !$options['reply_only']) {
//      $search_result->collections = entity_load('ting_collection', array(), array('ding_entity_id' => $ids));
//    }
//  }

  return $search_result;
}

function parse_search_results($results) {
  require './object.inc';
  $output = array();
  foreach ($results->collections as $collection) {
    
    //file_put_contents("/home/quickstart/work/debug/debugperf1.txt", print_r($collection , TRUE), FILE_APPEND);
    if (isset($collection->objects[0])) {
      $object = $collection->objects[0];
      $output[] = array(
       'title' => getTitle($object),
       'creators' => getCreators($object),
       'subjects' => getSubjects($object),
       'abstract' => getAbstract($object),
       'type' => getObjectType($object), //TODO multiple types
       'local_id' => getLocalId($object),
       'owner_id' => getOwnerId($object),
      );
    }   
  }
  return $output;
}

/**
 * Attempt to quote reserved words in a search query.
 *
 * As proper quoting would require a full CQL parser, we cheat and
 * just work on the part we know is the free text part.
 *
 * Also, we don't mess with uppercase reserved words.
 */
function _ting_search_quote($string) {
  if (preg_match('/^(.*?)(AND \(.*|$)/', $string, $rx)) {
    $keys = $rx[1];
    $new_keys = preg_replace_callback('/(?:(".*?(?<!\\\)")|\b(and|or|not|prox)\b)/i', '_ting_search_quote_callback', $keys);
    $string = preg_replace('/^' . preg_quote($keys, '/') . '/', $new_keys, $string);
  }
  return $string;
}


/**
 *
 */
function _ting_search_quote_callback($matches) {
  // If $matches[2] is empty, it's because the quote pattern
  // matched. Don't do anything with it.
  if (!empty($matches[2])) {
    // Boolean operator, but not uppercase, quote it.
    if ($matches[2] != drupal_strtoupper($matches[2])) {
      return '"' . $matches[2] . '"';
    }
    // Uppercase boolean operator, return as is.
    return $matches[2];
  }
  // We have a quote. Just return it.
  return $matches[1];
}

function ting_execute($request) {
  try {
    $client = new TingClient(new TingClientRequestAdapter(), new TingClientVoidLogger());
    $res = $client->execute($request);
    return $res;
  } catch (TingClientException $e) {
    var_dump($e);
   // watchdog('ting client', 'Error performing request: ' . $e->getMessage(), NULL, WATCHDOG_ERROR, 'http://' . $_SERVER["HTTP_HOST"] . request_uri());
    return FALSE;
  }
}

?>
