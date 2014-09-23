<?php

/**
 * Expose menu items as context reactions.
 */
class context_reaction_menu extends context_reaction {
  /**
   * Provide a form element that allow the admin to chose a menu item.
   */
  function options_form($context) {
    if (module_exists('menu')) {
      $menus = menu_parent_options(menu_get_menus(), array('mlid' => 0));
      $root_menus = array();
      foreach ($menus as $key => $name) {
        $id = explode(':', $key);
        if ($id[1] == '0') {
          $root_menus[$id[0]] = check_plain($name);
        }
        else {
          $link = menu_link_load($id[1]);
          $identifier = $link['link_path'];
          $root_menu = $root_menus[$id[0]];
          while (isset($menus[$root_menu][$identifier])) {
            $identifier .= "'";
          }
          $menus[$root_menu][$identifier] = $name;
        }
        unset($menus[$key]);
      }
      array_unshift($menus, "-- " . t('None') . " --");
    }
    else {
      $menus = array();
    }
    return array(
      '#title' => $this->title,
      '#description' => $this->description,
      '#options' => $menus,
      '#type' => 'select',
      '#default_value' => $this->fetch_from_context($context),
    );
  }

  /**
   * Override of options_form_submit().
   * Trim any identifier padding for non-unique path menu items.
   */
  function options_form_submit($values) {
    return trim($values, "'");
  }

  /**
   * Provide active trail in all menus in which our path appears.
   */
  function execute(&$vars = NULL) {
    $paths = array();
    foreach($this->get_active_paths() as $path) {
     $paths[] = $path;
    }
    if(count($paths)>0) {
      $query = db_select('menu_links', 'm')->fields('m', array('link_path', 'mlid', 'menu_name'))->condition('m.link_path', $paths, 'IN')->execute();
      while($menu = $query->fetchObject()) {
        $path = $menu->link_path;
        menu_tree_set_path($menu->menu_name, $path);
        $item = menu_link_load($menu->mlid);
        $route = array();
        while($item) {
          array_unshift($route, $item);
          $item = menu_link_load($item['plid']);
        }
        array_unshift($route, array(
          'title' => t('Home'),
          'href' => '<front>',
          'link_path' => '',
          'localized_options' => array(),
          'type' => 0,
        ));
        menu_set_active_trail($route);
      }
    }
  }

  function get_active_paths() {
    $active_paths = array();
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions[$this->plugin])) {
        $active_paths[] = $context->reactions[$this->plugin];
      }
    }
    return $active_paths;
  }
}
