<?php ?>
<form action="<?php print $facets['action']// /search/ting/dublin%20bog?>" method="post" id="ding-facetbrowser-form" accept-charset="UTF-8">
  <div><input type="hidden" name="search_key" value="<?php print $facets['search_key']?>" />

    <?php foreach ($facets['facet_types'] as $facet_type => $facet): ?> 
      <fieldset id="<?php print $facet['id']?>" data="<?php print $facet_type?>" count="<?php print $facet['count']?>" class="visible form-wrapper">
        <legend>
          <span class="fieldset-legend"><?php print $facet['title']?></span>
        </legend><div class="fieldset-wrapper">
          <div id="edit-type" class="form-checkboxes">
            <?php foreach ($facet['terms'] as $facet_item): ?> 
              <?php print $facet_item['markup']; ?>             
            <?php endforeach; ?>  
          </div>
        </div>
      </fieldset>          
    <?php endforeach; ?>  

    <input type="hidden" name="form_build_id" value="form-79MiniRzI458CTijHS5kfJzcfbEJkWBLXkdX9HdJvSQ" />
    <input type="hidden" name="form_id" value="ding_facetbrowser_form" />
  </div>
</form> 
