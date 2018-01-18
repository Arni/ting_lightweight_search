<?php

function theme_results ($results, $facets, $pager, $jquery_settings) {
  try {
  $vars = array();
  my_theme_search($results, &$vars);
  theme_facets($facets, &$vars);
  $vars['pager'] = $pager;
  $vars['jquery'] = get_jquery($jquery_settings);
  return my_theme('templates/search_page_template.php', $vars);  
  } catch (Exception $e) {
    return print_($e);
  }
}

function my_theme($template, $vars) {
  $path = DRUPAL_ROOT . '/profiles/ding2/modules/ting_lightweight_search/';

  ob_start();
  extract($vars);
  require $path . $template;
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
      $vars['title_link'] = my_l($result['title'], $collection_link);
    }

    $image_src = 'https://test.randersbib.dk/profiles/ding/modules/ting/modules/ting_covers/index.php?ownerid=' . $result['owner_id'] . '&objectid=' . $result['local_id'];
    $image = '<img src="' . $image_src . '">';
    $vars['image'] = my_l($image, $collection_link);

    if (isset($result['creators']) && !empty($result['creators'])) {
      $creators = 'Af ';
      foreach ($result['creators'] as $creator) {
        $creators .= my_l($creator, '/search/ting/dc.creator=' . $creator, "author"); //'<a class="author" href="/search/ting/dc.creator=' . $creator . '"> ' . $creator . '</a> ';
      }
      $vars['creators'] = $creators;
    }

    $vars['abstract'] = $result['abstract'];
    $vars['date'] = ' ( ' . $result['date'] . ')';

    if ($result['serie_title']) {
      $serie_search = check_plain(str_replace('@serietitle', $result['serie_title'], 'bib.titleSeries="@serietitle" OR dc.description="@serietitle"'));
      $vars['serie'] = my_l($result['serie_title'], '/search/ting/' . $serie_search, "series");
    } else if ($result['serie_description']) {
      $vars['serie'] = $result['serie_description'];
    }

    if ($result['subjects']) {
      $subjects = array();
      foreach ($result['subjects'] as $subject) {
        $subjects[] = my_l($subject, '/search/ting/dc.subject=' . $subject, "subject");
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

        $types[] = '<li id="' . $id . '" class="' . $class . '">' . my_l(availability_text($type['type']), $path) . '</li>';
      }
      $vars['types'] = $types;
    }
    $items[] = my_theme('templates/item-template.php', $vars);
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
      $markup .= my_l($link_text, $term['link']); //<a href="http://ding2tal.dev/search/ting/dublin%20bog?facets[]=facet.branch%3Adrb" title="drb">drb <span class="count">(19)</span></a> 
      $markup .= '</label></div>';
      $term['markup'] = $markup;
    }
    }
  }
  $vars['facet_browser'] =  my_theme('templates/facets-template.php', array('facets' => $facets));
}

function get_jquery($jquery_settings) {
  $availability =  ',"ding_availability":'. json_encode($jquery_settings['ding_availability']);
  $settings = 'jQuery.extend(Drupal.settings, {"basePath":"\/","pathPrefix":"","ajaxPageState":{"theme":"ddbasic","theme_token":"vwQCvCWHWBsjGRzt-PYVG6zPwKRwr-KVHvTgmJNqVs8","js":{"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/jquery\/1.7\/jquery.min.js":1,"misc\/jquery.once.js":1,"misc\/drupal.js":1,"profiles\/ding2\/modules\/contrib\/tipsy\/javascripts\/jquery.tipsy.js":1,"profiles\/ding2\/modules\/contrib\/tipsy\/javascripts\/tipsy.js":1,"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/ui\/ui\/minified\/jquery.ui.core.min.js":1,"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/ui\/ui\/minified\/jquery.ui.widget.min.js":1,"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/ui\/ui\/minified\/jquery.ui.position.min.js":1,"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/ui\/ui\/minified\/jquery.ui.autocomplete.min.js":1,"profiles\/ding2\/modules\/contrib\/jquery_update\/replace\/misc\/1.7\/states.js":1,"misc\/form.js":1,"public:\/\/languages\/da_4QW_xS3V912SSE4YQCW8YfTJ8AlXLaRVllWK1MWDB3c.js":1,"profiles\/ding2\/modules\/contrib\/panels\/js\/panels.js":1,"profiles\/ding2\/modules\/ding_toggle_format\/js\/ding_toggle_format.js":1,"profiles\/ding2\/modules\/ding_toggle_format\/js\/jquery.cookie.min.js":1,"profiles\/ding2\/modules\/ting_search\/ting_search_extendform.js":1,"profiles\/ding2\/modules\/ding_facetbrowser\/js\/jquery.cookie.min.js":1,"profiles\/ding2\/modules\/ding_availability\/js\/ding_availability.js":1,"profiles\/ding2\/modules\/ting_covers\/js\/ting-covers.js":1,"misc\/collapse.js":1,"profiles\/ding2\/modules\/contrib\/field_group\/field_group.js":1,"profiles\/ding2\/modules\/ding_facetbrowser\/js\/facetbrowser.js":1,"profiles\/ding2\/modules\/ting_search_autocomplete\/js\/ting_search_autocomplete.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/equalize.min.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/jquery.cookie.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/ddbasic.topbar.menu.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/ddbasic.common.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/respond.min.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/jquery.scrollto.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/html5shiv.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/selectivizr-min.js":1,"profiles\/ding2\/themes\/ddbasic\/scripts\/scalefix.js":1},"css":{"modules\/system\/system.base.css":1,"modules\/system\/system.menus.css":1,"modules\/system\/system.messages.css":1,"modules\/system\/system.theme.css":1,"profiles\/ding2\/modules\/contrib\/tipsy\/stylesheets\/tipsy.css":1,"misc\/ui\/jquery.ui.core.css":1,"misc\/ui\/jquery.ui.theme.css":1,"misc\/ui\/jquery.ui.autocomplete.css":1,"modules\/comment\/comment.css":1,"profiles\/ding2\/modules\/contrib\/date\/date_api\/date.css":1,"profiles\/ding2\/modules\/contrib\/date\/date_popup\/themes\/datepicker.1.7.css":1,"modules\/field\/theme\/field.css":1,"modules\/node\/node.css":1,"modules\/search\/search.css":1,"profiles\/ding2\/modules\/ting\/ting.css":1,"modules\/user\/user.css":1,"profiles\/ding2\/modules\/contrib\/views\/css\/views.css":1,"profiles\/ding2\/modules\/contrib\/ctools\/css\/ctools.css":1,"profiles\/ding2\/modules\/contrib\/panels\/css\/panels.css":1,"profiles\/ding2\/modules\/ding_toggle_format\/css\/ding_toggle_format.css":1,"profiles\/ding2\/modules\/ding_availability\/css\/ding_availability_legend.css":1,"profiles\/ding2\/modules\/ding_availability\/css\/ding_availability.css":1,"profiles\/ding2\/modules\/ding_facetbrowser\/css\/facetbrowser.css":1,"profiles\/ding2\/modules\/contrib\/field_group\/field_group.css":1,"profiles\/ding2\/modules\/ting_search_autocomplete\/css\/ting_search_autocomplete.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/ddbasic.reset.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/ddbasic.font.awesome.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/ddbasic.styles.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/system.menus.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/system.messages.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/addressfield.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/date.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/jquery.ui.theme.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/jquery.ui.theme.min.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/jquery.ui.dialog.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/jquery.ui.dialog.min.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/ddbasic.ie8.css":1,"profiles\/ding2\/themes\/ddbasic\/css\/ddbasic.ie9.css":1}},"tipsy":{"custom_selectors":[{"selector":".tipsy","options":{"fade":1,"gravity":"w","delayIn":0,"delayOut":0,"trigger":"hover","opacity":"0.8","offset":0,"html":0,"tooltip_content":{"source":"attribute","selector":"title"}}}]},"states":{"#edit-submit":{"visible":{":input[name=\u0022op\u0022]":{"value":""}}}},"field_group":{"div":"search_result"},"ding_availability_link":1,"dingFacetBrowser":{"mainElement":".pane-ding-facetbrowser","number_of_terms":"5"},"ddbasic":{"load_equalize":1}';
  $settings .=  $availability . '});';
  $jquery = array(
    'prefix' => '<!--//--><![CDATA[//><!--',
    'settings' => $settings,
    'postfix' => '//--><!]]>'
  );
  return $jquery;
}

function my_l($text, $path, $html_class = NULL) {
  if ($text && $path) {
    $link = '<a href="' . my_check_plain($path) . '"';
    if ($html_class) {
      $link .= ' class="' . $html_class . '" ';
    }
    $link .= '>' . $text . '</a>';
    return $link;
  }
}

function my_check_plain($text) {
  return htmlspecialchars($text, ENT_QUOTES, 'UTF-8');
}

?>