<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function load_inc() {
//$path = drupal_get_path('module', 'ting_ligtweight_search');
  $path = DRUPAL_ROOT . '/profiles/ding2/modules/ting_lightweight_search';

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

function search($query) {
  load_inc();
  $output = '';
  if ($query) {
    $search_results = ting_do_search($query);
    $results = parse_search_results($search_results);
    $facets = process_facets($search_results->facets, $query, '/search/ting/' . $query);
    $more_pages = ($search_results->more ? 1 : 0);
    $pager = make_pager(1, 1 + $more_pages, $query);
    $availability = process_availability($results);
    
    $output = theme_results($results, $facets, $pager, $availability);
    
    $search_context = get_search_context($search_results->facets, $search_results->numTotalObjects);
    var_dump($search_context);
    file_put_contents("/home/quickstart/work/debug/debugsearchcontext2.txt", print_r($search_context  , TRUE), FILE_APPEND);
  }

//header('Content-Type: application/json');
//$json_output = array("result_html" => $output, "availability" => $availability );
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
        $result = $prefix . my_l($title, '/search/ting/' . '"' . $title . '"');
      } else if (stripos($prefix, 'Samhørende:') !== FALSE) {

        $titles = $parts[1];
        // Multiple titles are separated by ' ; '. Explode to iterate over them
        $titles = explode(' ; ', $titles);
        foreach ($titles as &$title) {
          $title = trim($title);
          // Some title elements are actually volumne numbers. Do not convert these to links
          if (!preg_match('/(nr.)? \d+/i', $title)) {
            $title = my_l($title, '/search/ting/' . '"' . $title . '"');
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

function get_search_context($facets, $numTotalObjects) {
  $bib_zoom_count = $number_of_library_materials = get_term_count($facets, 'facet.acSource', array('bibzoom (album)')); 
  $numTotalObjects = $numTotalObjects - $bib_zoom_count;
  //file_put_contents("/home/quickstart/work/debug/debugsearchcontext1.txt", print_r($facets , TRUE), FILE_APPEND);
  $material_type = get_material_type($facets, $numTotalObjects);
  if ($material_type != 'book') {
    return $material_type; 
  }
  $age_group = get_age_group($facets);
  if ($age_group == 'children') {
    return get_children_genres($facets);;
  } else {
    $genre_category = get_genre_category($facets);
    if ($genre_category == 'fiction') {
      //file_put_contents("/home/quickstart/work/debug/debugsearchcontext9.txt", print_r(get_fiction_genres($facets) . ' ', TRUE), FILE_APPEND);
      return get_fiction_genres($facets);
    } else if ($genre_category == 'nonfiction') {
      return get_non_fiction_genres($facets);
    } 
  }

}

function get_material_type($facets, $total_number) {
  if (is_films($facets, $total_number))
    return 'film';
  else if (is_music($facets, $total_number))
    return 'music';
  else
    return 'book';
}

function is_films($facets, $total_number) {
   $film_types = array('film (net)', 'dvd', 'blue-ray');
   $number_of_films = get_term_count($facets, 'facet.type', $film_types);
   return evalute_condition($number_of_films, $total_number, 0.5);
}


function is_music($facets, $total_number) {
   $music_types = array('cd (musik)', 'grammofonplade', 'node');
   $number_of_music = get_term_count($facets, 'facet.type', $music_types );
   $number_of_library_materials = get_term_count($facets, 'facet.acSource', array('bibliotekets materialer'));
   return evalute_condition($number_of_music, $number_of_library_materials, 0.5);
}

function get_age_group($facets) {
   $number_of_adult = get_term_count($facets, 'facet.category',  array('voksenmaterialer'));
   $number_of_children = get_term_count($facets, 'facet.category', array('børnematerialer'));
   //file_put_contents("/home/quickstart/work/debug/debugsearchcontext6.txt", print_r($number_of_adult . ' ' . $number_of_children . ' : ', TRUE), FILE_APPEND);
   return ($number_of_adult > $number_of_children ) ? 'adult' : 'children';
}

function get_genre_category($facets) {
  $top_number_of_fiction = 0;
  $top_number_of_non_fiction = 0;
  if (isset($facets['facet.fiction']) && is_array($facets['facet.fiction']->terms)) {
    $top_number_of_fiction = reset($facets['facet.fiction']->terms);
  }
  if (isset($facets['facet.nonFiction']) && is_array($facets['facet.nonFiction']->terms)) {
    $top_number_of_non_fiction = reset($facets['facet.nonFiction']->terms);
  }
  //file_put_contents("/home/quickstart/work/debug/debugsearchcontext8.txt", print_r($top_number_of_fiction . ' ' . $top_number_of_non_fiction . ' : ', TRUE), FILE_APPEND);
  return ($top_number_of_fiction > $top_number_of_non_fiction ) ? 'fiction' : 'nonfiction';
}

function get_fiction_genres($facets) {
  $fiction_genres = array(
    'krimi' => array('krimi'),
    'kærlighed' => array('kærlighed', 'erotik', 'sex'),
    'fantasy' => array('science fiction', 'fantasy'),
  );
  return get_detail_genre($facets, $fiction_genres, 'facet.fiction', 'fiction');
}

function get_non_fiction_genres($facets) {
  $fiction_genres = array(
    'haver' => array('haver'),
    'kogebøger' => array('slankeretter', 'ernæring', 'diætretter', 'vegetarretter', 'kost'),
  );
  return get_detail_genre($facets, $fiction_genres, 'facet.nonFiction', 'nonfiction');
}

function get_children_genres($facets) {
  $fiction_genres = array(
    'piger' => array('piger'),
    'drenge' => array('drenge'),
  );
  return get_detail_genre($facets, $fiction_genres, 'facet.fiction', 'children');
}

function get_detail_genre($facets, $genres, $facet_name, $default_value) {
  $top_genre_count = 0;
  $top_genre_name = '';
  foreach ($genres as $genre_name => $genre) {
    $genre_count = get_top_term_count($facets, $facet_name, $genre);
    if ($genre_count > $top_genre_count) {
      $top_genre_count = $genre_count;
      $top_genre_name = $genre_name;
    }
  }
  if ($top_genre_count > 0) {
    return $top_genre_name;
  } else {
    return $default_value;
  }
}

function get_top_term_count($facets, $facet_name, $term_names = array()) {
  $number_of_terms = 0;
  if (isset($facets[$facet_name])) {
    $top_terms = array_slice($facets[$facet_name]->terms, 0, 3);
    foreach ($top_terms as $top_term => $term) {
      if (in_array($top_term, $term_names)) {
        $number_of_terms = $term;
        break;
      }
    }
  }
  return $number_of_terms;
}

function get_term_count($facets, $facet_name, $term_names = array()) {
  $number_of_terms = 0;
  if (isset($facets[$facet_name])) {
    $terms = $facets[$facet_name]->terms;  
    foreach ($term_names as $term_name) {
      if (isset($terms[$term_name])) {
        $number_of_terms += $terms[$term_name];
      }
    }
  }
  return $number_of_terms;
}

function evalute_condition($number_of_terms, $total_number, $pass_ratio) {
  if ($total_number != 0) {
  $term_ratio = $number_of_terms/$total_number;
  //file_put_contents("/home/quickstart/work/debug/debugsearchcontext4.txt", print_r($number_of_terms  . ' ' . $total_number . ' ' . $pass_ratio . ' ' . $term_ratio . ' | ', TRUE), FILE_APPEND);
  return ($term_ratio >  $pass_ratio);
  } else {
    return false;
  }
}



?>
