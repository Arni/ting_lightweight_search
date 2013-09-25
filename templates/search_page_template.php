<?php ?>

<div class="panel-pane pane-page-tabs" >



  <div class="pane-content">
    <div id="tabs"><h2 class="element-invisible">Primære faneblade</h2><ul class="tabs primary"><li class="active"><a href="/search/ting/dublin%20bog" class="active">Data well<span class="element-invisible">(aktiv fane)</span></a></li>
        <li><a href="/search/node/dublin%20bog">Hjemmeside</a></li>
      </ul></div>  </div>


</div>
<div class="panel-pane pane-page-content" >



  <div class="pane-content">
    <div  class="left-sidebar default-layout">
      <div class="layout-wrapper">
        <div class="primary-content">
          <div class="panel-pane pane-block pane-ting-search-search-display-extended-query" >



            <div class="pane-content">
              <p id="search-query-display">
                <span id="search-query-label">Din søgning:</span>
                <span id="search-query-string"></span>
              </p>
            </div>


          </div>
          <div class="panel-pane pane-ting-search" >



            <div class="pane-content">
              <div class="ting-search-amount-block">Show <em class="placeholder">1</em>-<em class="placeholder">10</em> of <em class="placeholder">130</em> results</div>  </div>


          </div>
          <div class="panel-pane pane-block pane-ting-search-sort-form" >



            <div class="pane-content">
              <form action="/search/ting/dublin%20bog" method="post" id="ting-search-sort-form" accept-charset="UTF-8"><div><div class="form-item form-type-select form-item-sort">
                    <label class="" for="edit-sort">Sortér efter: </label>
                    <select id="edit-sort" name="sort" class="form-select"><option value="" selected="selected">Relevans</option><option value="title_ascending">Titel a &gt; å</option><option value="title_descending">Titel å &gt; a</option><option value="creator_ascending">Forfatter a &gt; å</option><option value="creator_descending">Forfatter å &gt; a</option><option value="date_ascending">Udgivelsesår - ældste først</option><option value="date_descending">Udgivelsesår - nyeste først</option></select>
                    <div class="description">Set sort order for search result</div>
                  </div>
                  <input type="submit" id="edit-submit" name="op" value="Sortér" class="form-submit" /><input type="hidden" name="form_build_id" value="form-d9bhGZmucZaWPZunJTnDur7Gm6XkD-CX1AYRAAMzGfs" />
                  <input type="hidden" name="form_id" value="ting_search_sort_form" />
                </div></form>  </div>


          </div>
          <div class="panel-pane pane-block pane-ding-toggle-format-toggle" >



            <div class="pane-content">
              <span class="ding-toggle-format-label">Visningsformat<a href="/%23" title="Toggle format link" id="ding-toggle-format" class="ding-toggle-format-short"><img id="ding-toggle-format-image" src="http://ding2tal.dev/profiles/ding2/modules/ding_toggle_format/images/blank.gif" alt="Toggle format" title="Toggle format link" /></a>  </div>


          </div>
          <div class="panel-pane pane-block pane-ting-search-records-per-page" >



            <div class="pane-content">
              <div id="ting-search-records-per-page">Results on page:<a href="/search/ting/dublin%20bog?size=10&amp;page=0" class="selected active">10</a><a href="/search/ting/dublin%20bog?size=25&amp;page=0" class="active">25</a><a href="/search/ting/dublin%20bog?size=50&amp;page=0" class="active">50</a></div>  </div>


          </div>
          <div class="panel-pane pane-block pane-ding-availability-legend" >



            <div class="pane-content">
              <div class="availability-legend"><div class="availability-legend-item available"><img src="http://ding2tal.dev/profiles/ding2/modules/ding_availability/images/blank.gif" alt="Tilgængelig" title="Tilgængelig" /><span class="availability-label">Tilgængelig</span></div><div class="availability-legend-item on-loan"><img src="http://ding2tal.dev/profiles/ding2/modules/ding_availability/images/blank.gif" alt="Udlånt" title="Udlånt" /><span class="availability-label">Udlånt</span></div><div class="availability-legend-item unavailable"><img src="http://ding2tal.dev/profiles/ding2/modules/ding_availability/images/blank.gif" alt="Ikke tilgængelig" title="Ikke tilgængelig" /><span class="availability-label">Ikke tilgængelig</span></div><div class="availability-legend-item unreservable"><img src="http://ding2tal.dev/profiles/ding2/modules/ding_availability/images/blank.gif" alt="Kan ikke reserveres" title="Kan ikke reserveres" /><span class="availability-label">Kan ikke reserveres</span></div><div class="clearfix"></div></div>  </div>


          </div>
          <div class="panel-pane pane-search-result" >



            <div class="pane-content">
              <div class="search-results">
                <ul class="list floated">
                  <?php
                  if ($items) {
                    foreach ($items as $item) {
                      print $item;
                    }
                  }
                  ?> 
                </ul>
                <ul class="pager">
                  <?php if ($pager) : ?> 
                    <?php foreach ($pager as $pager_item): ?> 
                      <?php print $pager_item; ?>             
                    <?php endforeach; ?>  
                  <?php endif; ?>  

                </ul>

              </div>
            </div>


          </div>
        </div>
        <aside class="secondary-content">
          <div class="panel-pane pane-ding-facetbrowser" >

            <h2 class="pane-title">Filtrér din søgning</h2>


            <div class="pane-content">
              <?php
              if ($facet_browser) {
                print $facet_browser;
              }
              ?>
            </div>


          </div>
        </aside>
      </div>



    </div>  </div>


</div>
