<?php

function theme_results ($results, $facets, $pager) {
  $vars = array();
  my_theme_search($results, &$vars);
  theme_facets($facets, &$vars);
  $vars['pager'] = $pager;
  return my_theme('./templates/search_page_template.php', $vars);  
}

function my_theme($template, $vars) {
  ob_start();
  extract($vars);
  require $template;
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

function my_theme_search($results, $vars) {
  $items = array();
  foreach ($results as $result) {
    $vars = array();
    $collection_link = '/ting/collection/' . $result['owner_id'] . ':' . $result['local_id'];
    if (isset($result['title'])) {
      $vars['title_link'] = l($result['title'], $collection_link);
    }

    $image_src = 'https://test.randersbib.dk/profiles/ding/modules/ting/modules/ting_covers/index.php?ownerid=' . $result['owner_id'] . '&objectid=' . $result['local_id'];
    $image = '<img src="' . $image_src . '">';
    $vars['image'] = l($image, $collection_link);

    if (isset($result['creators']) && !empty($result['creators'])) {
      $creators = 'Af ';
      foreach ($result['creators'] as $creator) {
        $creators .= l($creator, '/search/ting/dc.creator=' . $creator, "author"); //'<a class="author" href="/search/ting/dc.creator=' . $creator . '"> ' . $creator . '</a> ';
      }
      $vars['creators'] = $creators;
    }

    $vars['abstract'] = $result['abstract'];
    $vars['date'] = ' ( ' . $result['date'] . ')';

    if ($result['serie_title']) {
      $serie_search = check_plain(str_replace('@serietitle', $result['serie_title'], 'bib.titleSeries="@serietitle" OR dc.description="@serietitle"'));
      $vars['serie'] = l($result['serie_title'], '/search/ting/' . $serie_search, "series");
    } else if ($result['serie_description']) {
      $vars['serie'] = $result['serie_description'];
    }

    if ($result['subjects']) {
      $subjects = array();
      foreach ($result['subjects'] as $subject) {
        $subjects[] = l($subject, '/search/ting/dc.subject=' . $subject, "subject");
      }
      $vars['subjects'] = $subjects;
    }
    //<li id="availability-77300027963390-dvd" class="availability dvd  pending first last"><a href="/ting/object/773000%3A27963390">Dvd</a></li>
    if ($result['types']) {
      $types = array();
      foreach ($result['types'] as $type) {
        $id = availability_id($type);
        $class = availability_class ($type['type']);
        if ($type['count'] == 1) {
          $path = '/ting/object/' . $type['owner_id'] . ':' . $type['local_id'];
        } else {
          $path = '/ting/collection/' . $type['owner_id'] . ':' . $type['local_id'];
        }

        $types[] = '<li id="' . $id . '" class="' . $class . '">' . l(availability_text($type['type']), $path) . '</li>';
      }
      $vars['types'] = $types;
    }
    $items[] = my_theme('./templates/item-template.php', $vars);
  }
  $vars['items'] = $items;
 
}

function theme_facets($facets, $vars) {
  foreach ($facets['facet_types'] as &$facet) {
    $facet_name = $facet['facet_type_name'];
    if (isset($facet['terms']) && is_array($facet['terms'])) {
    foreach ($facet['terms'] as $term_name => &$term) {
      $markup = '<div class="form-item form-type-checkbox form-item-' . $facet_name . '-' . $term_name . '">';
      $markup .= '<input type="checkbox" id="edit-' . $facet_name . '-' . $term_name . '" name="' . $facet_name . '[' . $term_name . ']" value="' . $term_name . '" class="form-checkbox" />';
      $markup .= '<label class="option" for="edit-' . $facet_name . '-' . $term_name . '"> ';
      $link_text = $term['term_name_title'] . ' <span class="count">(' . $term['term_count'] . ')</span>';
      $markup .= l($link_text, $term['link']); //<a href="http://ding2tal.dev/search/ting/dublin%20bog?facets[]=facet.branch%3Adrb" title="drb">drb <span class="count">(19)</span></a> 
      $markup .= '</label></div>';
      $term['markup'] = $markup;
    }
    }
  }
  $vars['facet_browser'] =  my_theme('./templates/facets-template.php', array('facets' => $facets));
}

function l($text, $path, $html_class = NULL) {
  if ($text && $path) {
    $link = '<a href="' . check_plain($path) . '"';
    if ($html_class) {
      $link .= ' class="' . $html_class . '" ';
    }
    $link .= '>' . $text . '</a>';
    return $link;
  }
}

function check_plain($text) {
  return htmlspecialchars($text, ENT_QUOTES, 'UTF-8');
}

?>