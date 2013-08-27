<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<li class="list-item search-result ding-format-long">
  <div class="ting-object view-mode-teaser clearfix">
    <div class="ting-object view-mode-search-result clearfix">
      <div class="field-group-format group_ting_left_col_search field-group-div group-ting-left-col-search ting-object-left speed-none effect-none">  
        <div class="ting-cover ting-cover-object-id-29238154 ting-cover-style-ding_list_medium ting-cover-owner-id-773000 ting-cover-processed">
           <?php print $image ?>
        </div></div>
      <div class="field-group-format group_ting_right_col_search field-group-div group-ting-right-col-search ting-object-right speed-none effect-none"> 
        <div class="heading">
          <h2>
            <?php print $title_link ?>
            
          </h2>
        </div>
        <div class="content">
           <?php if (isset($creators)) { print $creators; }?>
           <?php if (isset($date)) { print $date; }?>
        </div>
        <div class="content">
           <?php if (isset($abstract)) { print $abstract; }?>
        </div>
        <?php if (isset($serie)) : ?>
        <div class="field field-name-ting-series field-type-ting-series field-label-inline clearfix">
          <div class="field-label">Series:&nbsp;</div>
          <div class="field-items">
            <div class="field-item even">
              <?php  print $serie; ?>
              : </div>
          </div>
        </div>
        <?php  endif ; ?>
        <?php if (isset($subjects)) : ?>
        <div class="subjects content">
          <?php foreach ($subjects as $subject) 
            print $subject;            
          ?>         
        </div>
        <?php  endif ; ?>
      </div></div>
    <?php if (isset($types)) : ?>
    <div class="availability content">
      <ul>
          <?php foreach ($types as $type): ?> 
            <?php print $type; ?>             
          <?php endforeach; ?>  
      </ul> 
    </div>
    <?php  endif ; ?>
  </div>
</li>
