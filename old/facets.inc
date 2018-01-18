<?php

function process_facets($facets = FALSE, $search = FALSE, $search_url) {
  $result_facets = array();
  $result_facets ['search_key'] = $search;
  $result_facets ['action'] = $search_url;

  // Building the facets fieldsets
  $i = 0;
  $facet_show_count = 10;

  foreach (configured_facets() as $configured_facet) {

    if (isset($facets[$configured_facet['name']])) {
      $facet = $facets[$configured_facet['name']];

      // Decide if the facet is to have the hidden class or not
      $facet_class = ($i >= $facet_show_count) ? 'hidden' : 'visible';

      if (count($facet->terms) > 0) {
        $result_facets['facet_types'][$facet->name] = array(
          'title' => check_plain($configured_facet['title']),
          'id' => str_replace('.', '-', $facet->name),
          'data' => array($facet->name),
          'count' => count($facet->terms),
          'class' => array($facet_class),
        );

        if (!empty($configured_facet['sorting'])) {
          switch ($configured_facet['sorting']) {
            case 'alphabetical' :
              ksort($facet->terms);
              break;
            case 'alphabetical_reverse' :
              krsort($facet->terms);
              break;
            case 'numeric' :
              ksort($facet->terms, SORT_NUMERIC);
              break;
            case 'numeric_reverse' :
              krsort($facet->terms, SORT_NUMERIC);
              break;
            default :
              break;
          }
        }
        $facet_name = explode('.', $facet->name);
        $element_name = end($facet_name);
        $result_facets['facet_types'][$facet->name]['facet_type_name'] = $element_name;
        foreach ($facet->terms as $term_name => $term_count) {
          $facets_string = $facet->name . ':' . $term_name;
          $term_name_title = $term_name;
          // Check if term name is longer than 14 chars,
          // if so, short it down to fit facetbrowser
          $substr_length = 19
            + substr_count(substr($term_name, 0, 20), 'l')
            + strtolower(substr_count(substr($term_name, 0, 20), 'i'))
            - strlen((string) $term_count);
          if (strlen($term_name) > $substr_length + 3) {
            $term_name_title = substr($term_name, 0, $substr_length) . '...';
          }
//
//          if (_isFacetSet($facet->name, $term_name) === TRUE) {
//            $default_values[] = $term_name;
//            $facets_string = '-' . $facets_string;
//
//            $selected[$term_name] = l($term_name_title . ' <span class="count">(' . $term_count . ')</span>',  $base_root . _parseRequestUri(), array(
//              'html' => TRUE,
//              'query' => array(
//                'facets' => array('' => $facets_string),
//              ),
//              'attributes' => array(
//                'title' => $term_name,
//              ),
//            ));
//          }
//          else {
//            $not_selected[$term_name] = l($term_name_title . ' <span class="count">(' . $term_count . ')</span>',  $base_root . _parseRequestUri(), array(
//              'html' => TRUE,
//              'query' => array(
//                'facets' => array('' => $facets_string),
//              ),
//              'attributes' => array(
//                'title' => $term_name,
//              ),
//            ));
          //         }

          $result_facets['facet_types'][$facet->name]['terms'][$term_name] = array(
            'term_name' => $term_name,
            'term_name_title' => $term_name_title,
            'term_count' => $term_count,
            'link' => $search_url . '?facets[]=' . $facet->name . '%3A' . $term_name,
          );
        }
      }
      $i++; // Loop is done so time to up that counter
    }
  }

  return $result_facets;
}

function configured_facets() {
  return array(
    0 => array(
      'name' => 'facet.type',
      'title' => 'Materialetype',
      'sorting' => 'default',
      'weight' => '-10',
    ),
    1 => array(
      'name' => 'facet.creator',
      'title' => 'Forfatter',
      'sorting' => 'default',
      'weight' => '-9',
    ),
    2 => array(
      'name' => 'facet.subject',
      'title' => 'Emne',
      'sorting' => 'default',
      'weight' => '-8',
    ),
    3 => array(
      'name' => 'facet.language',
      'title' => 'Sprog',
      'sorting' => 'default',
      'weight' => '-7',
    ),
    4 => array(
      'name' => 'facet.category',
      'title' => 'Målgruppe',
      'sorting' => 'default',
      'weight' => '-6',
    ),
    5 => array(
      'name' => 'facet.date',
      'title' => 'Årstal',
      'sorting' => 'numeric_reverse',
      'weight' => '-5',
    ),
    6 => array(
      'name' => 'facet.acSource',
      'title' => 'Kilde',
      'sorting' => 'default',
      'weight' => '-4',
    ),
    7 => array(
      'name' => 'facet.branch',
      'title' => 'Filial',
      'sorting' => 'alphabetical',
      'weight' => '-3',
    ),
  );
}

/**
 * Check the URI and drop pager parameter, if available.
 *
 * @return String
 */
function _parseRequestUri() {
  $new_query_part = array();
  if (stristr(request_uri(), '?')) {
    $request_uri_parts = explode('?', request_uri());

    $query = rawurldecode(end($request_uri_parts));
    $query_parts = explode('&', $query);
    foreach ($query_parts as $key => $part) {
      if (preg_match('/^page\=/', $part)) {
        unset($query_parts[$key]);
        continue;
      }
      $new_query_part[] = $part;
    }
    $query = $new_query_part;
    return $request_uri_parts[0] . '?' . implode('&', $query);
  } else {
    return request_uri();
  }
}

?>