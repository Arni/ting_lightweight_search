<?php

function make_pager($current_page, $total_pages, $query) {
  $quantity = 9;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $current_page;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $total_pages;
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.


  if ($pager_current > 1) {
    $li_previous = make_pager_element('pager_previous', $pager_current - 1, "Gå til forrige side" , "< forrige", $query);
  } else {
    $li_previous = "&nbsp;";
  }
  $li_first = make_pager_element('pager-first', 1 , "Gå til første side" , "« første", $query); 
  $li_next = make_pager_element('pager-next', $pager_current + 1, "Gå til næste side" , "næste >", $query);  
  $li_last = make_pager_element('pager-last', $pager_max , "Gå til sidste side" , "sidste »", $query);  
  $li_ellipsis = '<li class="pager-ellipsis">…</li>';
  $li_current = '<li class="pager-current">' . $pager_current. '</li>';

  if ($pager_max > 1) {
    if ($pager_current > 2) {
      $items[] = $li_first;
    }

    $items[] = $li_previous;

    // When there is more than one page, create the pager list.
    if ( $i != $pager_max) {
      if ($i > 1) {
        $items[] = $li_ellipsis;
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = make_pager_element('pager-item', $i , "Gå til side" . $i , $i, $query); 
        }
        if ($i == $pager_current) {
          $items[] = $li_current; 
        }
        if ($i > $pager_current) {
          $items[] = make_pager_element('pager-item', $i , "Gå til side" . $i , $i, $query); 
        }
      }
      if ($i < $pager_max) {
        $items[] = $li_ellipsis;
      }
    }
    else {
      $items[] = $li_current; 
    }

    $items[] = $li_next;
    if ($pager_current + 1 < $pager_max && $li_last) {
      $items[] = $li_last;
    }
    return $items;
}
}

function make_pager_element($class, $page, $page_title_text , $page_text, $query) {
   $page = $page - 1;
   $li = '<li class="' . $class .'"><a href="/search/ting/' . $query . '?page=' . $page . '" title="'. $page_title_text . '">' . $page_text . '</a></li>';
   return $li;
}


?>