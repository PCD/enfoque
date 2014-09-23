<?php
/**
 * @file
 * Admin page callback file for the simplified_social_share module.
 */

/**
 * Implements Social Login setting for the socialsharing module.
 */
function simplified_social_share_admin_settings($form, &$form_state) { 
  global $base_url;
  $my_path = drupal_get_path('module', 'simplified_social_share');
  drupal_add_js('http://code.jquery.com/ui/1.10.0/jquery-ui.js');
  drupal_add_js(drupal_get_path('module', 'simplified_social_share') . '/simplified_social_share.js');
  drupal_add_css(drupal_get_path('module', 'simplified_social_share') . '/simplified_social_share.admin.css');
  drupal_add_js('jQuery(document).ready(function() { 
    var horizontal_image="' . variable_get("socialsharing_horizontal_images") . '";
    var vertical_image="' . variable_get("socialsharing_vertical_images") . '";
    if(horizontal_image == 8 || horizontal_image == 9) {
      counter_horizntal_show();
    }
    else {
      sharing_horizonatl_show();
	  if(horizontal_image == 2 || horizontal_image == 3) {
        sharing_simplehorizonatl_show();
      } 
    }
    if(vertical_image == 6 || vertical_image == 7) {
      counter_vertical_show();
    }
    else {
      sharing_vertical_show();
    }
	var selected_theme = "' . variable_get("socialsharing_selected_share_interface") . '";
	if(selected_theme == "vertical"){
	  hidden_horizontal_widget();
	}
	else {
	  display_horizontal_widget();
	}
  }
  );
  function loginRadiusSaveLRSite(){
    if (jQuery("#lrSites").val().trim() == ""){
      jQuery("#loginRadiusMessage").html("Please select a site").css("color", "red");
      return;
    }
    // processing message
    jQuery("#loginRadiusMessage").html("<img width=20 height=20 src=' . $base_url . '/' . $my_path . '/images/loading.gif style=float:left;margin-right:5px; /><span style=color:blue;width:auto>Please wait. This may take a few minutes ...</span>");
    jQuery.ajax({
      type: "POST",
      url:  "' . url("simplified_social_share/lr_Login_submit") . '",
      data: {
        apikey: jQuery("#lrSites").val().trim()
      },
      success: function(data, textStatus, XMLHttpRequest){
        if (data.message == "success"){
          // refresh the page
          location.href = location.href;
        }
        else {
          // unexpected error
          jQuery("#loginRadiusMessage").html("Unexpected error occurred").css("color", "red");
        }
      },
      error: function(a, b, c){
        alert(JSON.stringify(a, null, 4)+"\n"+b+"\n"+c)
      }
    });
  }
  function lr_Login_submit(){
    // form validation
    var email = jQuery("#lr_username").val().trim();
    if (email == "" || jQuery("#lr_password").val().trim() == "" || (jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none" &&      jQuery("#lr_site").val().trim() == "")){
      jQuery("#loginRadiusMessage").html("Please fill all the fields.").css("color", "red");
      return;
    }
    // email validation
    var atPosition = email.indexOf("@");
    var dotPosition = email.lastIndexOf(".");
    if (atPosition < 1 || dotPosition < atPosition+2 || dotPosition+2>=email.length){
      jQuery("#loginRadiusMessage").html("Please enter a valid email address.").css("color", "red");
      return;
    }
    //password length validation
   if (jQuery("#lr_password").val().length < 6 || jQuery("#lr_password").val().length > 32 ) {
     jQuery("#loginRadiusMessage").html("Password length should be minimum of 6 characters and maximum 32 characters").css("color", "red");
     return;
   }
  // confirm password validation
  if (jQuery(".form-item.form-type-password.form-item-socialsharing-form-confirm-password").css("display") != "none" && !lr_confirm_password_validate()){
    return;
  }
  //Site Name validation
  if (jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none" && jQuery("#lr_site").val().match(/[.]/g)) {
    jQuery("#loginRadiusMessage").html("Symbol . not allowed.").css("color", "red");
    return;
  }
  if (jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none" && jQuery("#lr_site").val().match(/[_]/g)) {
    jQuery("#loginRadiusMessage").html("Symbol _ not allowed.").css("color", "red");
    return;
  }
  if(jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none"  && jQuery("#lr_site").val().length < 4 ) {
    jQuery("#loginRadiusMessage").html("Site name must be longer than three characters.").css("color", "red");
    return;
  }
  var url = "https://" + jQuery("#lr_site").val().trim() + ".hub.loginradius.com";
  var regularExpression = "^(ht|f)tp(s?)\:\/\/[0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*(:(0-9)*)*(\/?)([a-zA-Z0-9\-\.\?\,\"\/\\\+&%\$#_]*)?$";
  if (jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none" && !url.match(regularExpression)) {
    jQuery("#loginRadiusMessage").html("Site Name is not valid.").css("color", "red");
    return;
  }
  // processing message
  jQuery("#loginRadiusMessage").html("<img width=20 height=20 src=' . $base_url . '/' . $my_path . '/images/loading.gif style=float:left;margin-right:5px; /><span style=color:blue;width:auto;>Please wait. This may take a few minutes...</span>");
  var simplified_social_share_lr_Login_submit = function(data){
    if(data.status == 0){
      // show the message
      jQuery("#loginRadiusMessage").html(data.message).css("color", "red");
     }
     else if(data.status == 1 && data.message == "registration successful"){
     // refresh the page
       location.href = location.href;
     }
     else if(data.status == 1 && data.message == "login successful"){
       // display the app list
       var html = "<h3 id=loginRadiusFormTitle>Site Selection</h3><div id=loginRadiusFormDiv><label for=lrSites>Select a LoginRadius site</label><select id=lrSites class=form-select><option value=>--Select a Site--</option>";
       for(var i = 0; i < data.result.length; i++){
         html += "<option value= "+data.result[i].apikey+" >"+data.result[i].appName+"</option>";
       }
       html += "</select><br/>";
       html += "<input type=button id=loginRadiusLRSiteSave class=form-submit value=Save style=margin-top:28px;margin-right:40px;float:left;  /><div id=loginRadiusMessage style=margin-top:28px;></div></div>";
       jQuery("#edit-login-register").html(html);
       document.getElementById("loginRadiusLRSiteSave").onclick = function(){
       loginRadiusSaveLRSite();
     };
    }
  }
  var dataObject = {
    UserName: jQuery("#lr_username").val().trim(),
    Password: jQuery("#lr_password").val().trim(),
  };
  if (jQuery(".form-item.form-type-textfield.form-item-socialsharing-form-lr-site").css("display") != "none"){
   dataObject.lr_site = jQuery("#lr_site").val().trim();
  }
  jQuery.ajax({
      type: "POST",
      url:  "' . url("simplified_social_share/lr_Login_submit") . '",
      data: dataObject,
      dataType: "json",
      success: simplified_social_share_lr_Login_submit,
      error: function(a, b, c){
        alert(JSON.stringify(a, null, 4)+"\n"+b+"\n"+c)
      }
    });
  }', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
  $my_path = drupal_get_path('module', 'simplified_social_share');
  global $base_url;
  $thanks_text = t('<a href="@loginradius" target="_blank">LoginRadius</a> provides <a href="@social-login" target="_blank">Social Login</a>, <a href="@social-login" target="_blank">Social Share</a>, <a href="@social-login" target="_blank">Friend Invite</a>, <a href="@social-login" target="_blank">User Social Profile Data</a>, <a href="@user-profile-access" target="_blank">User Profile Access</a>, <a href="@social-login" target="_blank">Single Sign-On</a>, <a href="@social-login" target="_blank">Social Analytics</a> as single Unified API.<br/><br/> We also have ready to use plugins for <a href="@wordpress" target="_blank">Wordpress</a>, <a href="@joomla" target="_blank">Joomla</a>, <a href="@magento" target="_blank">Magento</a>, <a href="@oscommerce" target="_blank">osCommerce</a>,  <a href="@zencart" target="_blank">Zen-Cart</a>, <a href="@xcart" target="_blank">X-Cart</a>, <a href="@prestashop" target="_blank">Prestashop</a>, <a href="@vanillaforum" target="_blank">VanillaForum</a>, <a href="@vbulletin" target="_blank">vBulletin</a>.', array('@loginradius' => 'http://www.Loginradius.com/', '@joomla' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#joomlaextension', '@wordpress' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#wordpressplugin', '@vbulletin' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#vbulletinplugin', '@vanillaforum' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#vanillaaddons', '@magento' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#magentoextension', '@oscommerce' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#osCommerceaddons', '@prestashop' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#prestashopmodule', '@xcart' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#xcartextension', '@zencart' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#zencartplugin', '@dotnetnuke' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#dotnetnukemodule', '@blogengine' => 'https://www.loginradius.com/loginradius-for-developers/loginRadius-cms#blogengineextension', '@get_sociallogin' => 'http://support.loginradius.com/customer/portal/articles/593954', '@social-login' => 'https://www.loginradius.com/loginradius/product-overview'));
  $update_text = t('Module Version: 1.1 <br/> Author : LoginRadius <br/> Website: <a href="@loginradius" target="_blank">https://www.loginradius.com</a> <br/> Community : <a href="@community" target="_blank">http://community.loginradius.com</a> <br/>To receive updates on new features, future releases and other updates, please connect with us on<br/>
  <a href="@facebook" target="_blank">Facebook</a>, <a href="@twitter" target="_blank">Twitter</a> , <a href="@linkedin" target="_blank">LinkedIn</a>
', array('@loginradius' => 'https://www.Loginradius.com/', '@community' => 'http://community.loginradius.com', '@facebook' => 'https://www.facebook.com/loginradius', '@twitter' => 'https://twitter.com/LoginRadius', '@linkedin' => 'http://www.linkedin.com/company/2748191?trk=tyah'));
    $form['thanks_block'] = array(
     '#type' => 'fieldset',
    '#title' => t('Thank you for installing the LoginRadius Simplified Social Share module!'),
    '#description' =>  $thanks_text,
     '#attributes' => array('style' => 'float:left'),
  );
  $form['update_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Get Updates'),
    '#description' =>  $update_text
  );
  $api_key =trim(variable_get('socialsharing_api_key'));
  if (!isset($api_key) || empty( $api_key)) {
   $group = 'login_register';
   $form[$group] = array(
    '#type' => 'fieldset',
    '#attributes' => array('style' => 'clear:both'),
   );
   $form[$group]['socialsharing_form_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#id' => 'lr_username',
    '#prefix' => '<div style = "margin-bottom:5px;margin-top: -30px;"><h4 id = "lr_form_title">' . t('Login to your LoginRadius Account to change settings as per your requirements!') . '</h4></div>',
    '#default_value' => variable_get('socialsharing_form_username', ''),
  );
    $form[$group]['socialsharing_form_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#id' => 'lr_password',
    '#default_value' => variable_get('socialsharing_form_password', ''),
  );
   $form[$group]['socialsharing_form_confirm_password'] = array(
    '#type' => 'password',
    '#title' => t('Confirm Password'),
    '#id' => 'lr_confirm_password',
    '#default_value' => variable_get('socialsharing_form_confirm_password', ''),
  );
   $form[$group]['socialsharing_form_lr_site'] = array(
    '#type' => 'textfield',
    '#title' => t('LoginRadius Site'),
    '#id' => 'lr_site',
    '#default_value' => variable_get('socialsharing_form_lr_site', ''),
  );
  $form[$group]['lr_submit'] = array(
    '#type' => 'item',
    '#markup' => '<input type="button" id="lr_submit" name="lr_submit" value="Login" onclick= "lr_Login_submit();" class="form-submit"  style="margin-right:40px;float:left;" /><div id=loginRadiusMessage></div>',
   '#suffix' => '<div style="clear:both;"><a style="text-decoration:none" id="lr_toggle_form_link" href="javascript:void(0)" onclick=lr_toggle_form("login")>New to LoginRadius, Register Now!</a><br/><a style="text-decoration:none" target="_blank" href="https://www.loginradius.com/login/forgotten" >Forgot your password?</a></div>',
    );
  }
  else {
    $group = 'share';
    $form[$group] = array(
      '#type' => 'fieldset',
      '#title' => t('Social Sharing SETTINGS'),
      '#attributes' => array('style' => 'clear:both'),
    );
    $form[$group]['socialsharing_enable_share'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Social Sharing for your website?'),
      '#default_value' => variable_get('socialsharing_enable_share', 1),
      '#options' => array(
         1 => t('YES'),
         0 => t('NO'),
      )
    );
    $form[$group]['socialsharing_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API Key'),
      '#default_value' => variable_get('socialsharing_api_key', ''),
      '#description' => t('Insert LoginRadius API Key ( <a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a> )', array('@get_socialloginandsocialshare' => 'http://support.loginradius.com/customer/portal/articles/677100-how-to-get-loginradius-api-key-and-secret')),
    );
    $form[$group]['socialsharing_horizontal'] = array(
      '#type' => 'item',
      '#prefix' => '<div><b>' . t('What Social Sharing widget theme do you want to use across your website?<div class="description">Horizontal and Vertical themes can be enabled simultaneously</div>') . '</b></div>',
      '#markup' => l(t('Horizontal'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialsharing_horizontal', 'onclick' => 'display_horizontal_widget();'))) . "<b>|</b>" . l(t('Vertical'), "javascript:void(0)", array('fragment' => '', 'external' => TRUE, 'attributes' => array('id' => 'socialsharing_veritical', 'onclick' => 'hidden_horizontal_widget();' )))
    );
    $form[$group]['socialsharing_selected_share_interface'] = array(
      '#type' => 'hidden',
      '#title' => t('selected share interface'),
      '#default_value' => variable_get('socialsharing_selected_share_interface'),
      '#suffix' => '<div id=lrsharing_divwhite></div><div id=lrsharing_divgrey></div><div id="show_horizontal_block">',
    );
    $form[$group]['socialsharing_enable_horizontal_share'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Horizontal Social Sharing for your website?'),
      '#default_value' => variable_get('socialsharing_enable_horizontal_share', 1),
      '#options' => array(
         1 => t('YES'),
         0 => t('NO'),
      )
    );
    $form[$group]['socialsharing_enable_vertical_share'] = array(
      '#type' => 'radios',
      '#title' => t('Do you want to enable Vertical  Social Sharing for your website?'),
      '#default_value' => variable_get('socialsharing_enable_vertical_share', 1),
      '#options' => array(
         1 => t('YES'),
         0 => t('NO'),
       )
    );
    $form[$group]['socialsharing_label_string'] = array(
      '#type' => 'textfield',
      '#title' => t('What text do you want to display above the Social Sharing widget?'),
      '#default_value' => variable_get('socialsharing_label_string', 'Share it now!'),
      '#description' => t('Leave empty for no text.'),
    );
    $form[$group]['socialsharing_horizontal_images'] = array(
      '#type' => 'radios',
      '#id' => 'socialsharing_horizontal_images',
      '#default_value' => variable_get('socialsharing_horizontal_images', 0),
      '#options' => array(
         0 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing32.png"></img>',
         1 => '<img src="' . $base_url . '/' . $my_path . '/images/horizonSharing16.png"></img>',
         2 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-large.png"></img>',
         3 => '<img src="' . $base_url . '/' . $my_path . '/images/single-image-theme-small.png"></img>',
         8 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontalvertical.png"></img>',
         9 => '<img src="' . $base_url . '/' . $my_path . '/images/horizontal.png"></img>',
       ),
    );
    $form[$group]['socialsharing_vertical_images'] = array(
      '#type' => 'radios',
      '#id' => 'socialsharing_vertical_images',
      '#default_value' => variable_get('socialsharing_vertical_images', 6),
      '#options' => array(
         4 => '<img src="' . $base_url . '/' . $my_path . '/images/32VerticlewithBox.png"></img>',
         5 => '<img id="VerticlewithBox" src="' . $base_url . '/' . $my_path . '/images/16VerticlewithBox.png"></img>',
         6 => '<img src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-vertical.png"></img>',
         7 => '<img id="hybrid-verticle-horizontal"  src="' . $base_url . '/' . $my_path . '/images/hybrid-verticle-horizontal.png"></img>',
       ),
       '#attributes' => array('style' => 'display:inline'),
    );
    $form[$group]['socialsharing_show_horizotal']= array(
      '#type' => 'hidden',
      '#suffix' => '<div id="socialsharing_show_horizotal_widget">',
    );
     $counter_providers =variable_get('socialsharingcounter_rearrange_providers_list');
    if (empty($counter_providers)) {
      variable_set('socialsharingcounter_rearrange_providers_list', array( "0" => "Facebook Like", "1" => "Google+ +1", "2" => "Pinterest Pin it" , "3" => "LinkedIn Share", ));
    }
    $form[$group]['socialsharing_counter_show_providers_list'] = array(
      '#type' => 'item',
       '#id' => 'socialcounter_show_providers_list',
      '#class' => 'form-item form-type-checkboxes form-item-socialshare-counter-show-providers-list',
      '#title' => t('What Sharing Networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => variable_get('socialsharingcounter_rearrange_providers_list'),
      '#suffix' => '<div id="socialcounter_hidden_field" style="display:none;">',
    ); 
    foreach (variable_get("socialsharingcounter_rearrange_providers_list") as $provider) {
      if (!empty($provider)) {
        $raw = $provider;
        $provider = str_replace(' ', '', $provider);
        $provider = str_replace("++", "plusplus", $provider);
        $provider = str_replace("+", "plus", $provider);
        $form[$group][$provider] = array(
          '#type' => 'item',
          '#attributes' => array('class' => array('lrshare_' . $provider)),
          '#markup' => '<input type = "hidden" id = "input-lrcounter-' . $provider . '" name = "socialsharingcounter_rearrange_providers_list[]" value = "' . $raw . '" />',
         );
      }
    }
    $share_providers =variable_get("socialsharing_rearrange_providers_list");
    if (empty($share_providers)) {
      variable_set('socialsharing_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", "2" => "google", "3" => "pinterest", "4" => "twitter", ));
    }
    $form[$group]['socialsharing_show_providers_list'] = array(
      '#type' => 'item',
      '#id' => 'socialsharing_providers_list',
      '#title' => t('What Sharing Networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => variable_get('socialsharing_rearrange_providers_list'),
      '#prefix' => '</div><div id="loginRadiusSharingLimit" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
      '#suffix' => '<div id="rearrange_sharing_text"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialsharing_rearrange_providers" class="socialsharing_rearrange_providers">',
    );
    foreach (variable_get("socialsharing_rearrange_providers_list") as $provider) {
      if (!empty($provider)) {
        $form[$group]['lrshare_iconsprite32' . $provider] = array(
          '#type' => 'item',
          '#attributes' => array('class' => array('lrshare_' . $provider)),
          '#markup' => '<li id = "edit-lrshare-iconsprite32' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
          <input type = "hidden" id = "input-lrshare-' . $provider . '" name = "socialsharing_rearrange_providers_list[]" value = "' . $provider . '" /></li>', 
        );
      }
    }
    $form[$group]['socialsharing_rearran_providers_list']= array(
      '#type' => 'hidden',
      '#prefix' => '</ul><div id="socialsharing_question"></div>',
      '#suffix' => '</div>',
    ); 
    $form[$group]['socialsharing_show_veritical']= array(
      '#type' => 'hidden',
      '#suffix' => '<div id="socialsharing_show_veritcal_widget">',
    );
    $counter_providers =variable_get('socialsharingcounter_vertical_rearrange_providers_list');
    if (empty($counter_providers)) {
      variable_set('socialsharingcounter_vertical_rearrange_providers_list', array( "0" => "Facebook Like", "1" => "Google+ +1", "2" => "Pinterest Pin it" , "3" => "LinkedIn Share", ));
    }
    $form[$group]['socialsharing_counter_vertical_show_providers_list'] = array(
      '#type' => 'item',
      '#id' => 'socialcounter_vertical_show_providers_list',
      '#title' => t('What Sharing Networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => variable_get('socialsharingcounter_vertical_rearrange_providers_list'),
      '#suffix' => '<div id="socialcounter_vertical_hidden_field" style="display:none;">',
    );
    foreach (variable_get("socialsharingcounter_vertical_rearrange_providers_list") as $provider) {
      if (!empty($provider)) {
        $raw = $provider;
        $provider = str_replace(' ', '', $provider);
        $provider = str_replace("++", "plusplus", $provider);
        $provider = str_replace("+", "plus", $provider);
        $form[$group]['lrshare_vertical_' . $provider] = array(
          '#type' => 'item',
          '#attributes' => array('class' => array('lrshare_vertical_' . $provider)),
          '#markup' => '<input type = "hidden" id = "input-lrcounter-vertical-' . $provider . '" name = "socialsharingcounter_vertical_rearrange_providers_list[]" value = "' . $raw . '" />',
        );
      }
    }
    $share_providers =variable_get("socialsharing_vertical_rearrange_providers_list");
    if (empty($share_providers)) {
      variable_set('socialsharing_vertical_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", "2" => "google", "3" => "pinterest", "4" => "twitter", ));
    }
    $form[$group]['socialsharing_vetical_show_providers_list'] = array(
      '#type' => 'item',
      '#id' => 'socialsharing_vetical_show_providers_list',
      '#title' => t('What Sharing Networks do you want to show in the sharing widget? (All other sharing networks will be shown as part of LoginRadius sharing icon)'),
      '#default_value' => variable_get('socialsharing_vertical_rearrange_providers_list'),
      '#prefix' => '</div><div id="loginRadiusSharingLimit_vertical" style="color:red; display:none; margin-bottom: 5px;">' . t('You can select only 9 providers.') . '</div>',
      '#suffix' => '<div id="rearrange_sharing_text_vertical"><b>' . t('What sharing network order do you prefer for your sharing widget?(Drag around to set the order)') . '</b></div><ul id="socialsharing_vertical_rearrange_providers" class="socialsharing_vertical_rearrange_providers">',
    );
    foreach (variable_get("socialsharing_vertical_rearrange_providers_list") as $provider) {
      if (!empty($provider)) {
        $form[$group]['lrshare_iconsprite32_vertical' . $provider] = array(
          '#type' => 'item',
          '#attributes' => array('class' => array('lrshare_' . $provider)),
          '#markup' => '<li id = "edit-lrshare-iconsprite32_vertical' . $provider . '" class = "lrshare_iconsprite32 lrshare_' . $provider . '" title = "' . $provider . '" >
          <input type = "hidden" id = "input-lrshare-vertical-' . $provider . '" name = "socialsharing_vertical_rearrange_providers_list[]" value = "' . $provider . '" /></li>', 
        );
      }
    }
    $form[$group]['socialsharing_vertical_rearra_providers_list']= array(
      '#type' => 'hidden',
      '#prefix' => '</ul><div id="socialsharing_question"></div>',
      '#suffix' => '</div>',
    );
    $form[$group]['socialsharing_vertical_images']['socialsharing_vertical_position'] = array(
       '#type' => 'radios',
       '#title' => t('Select the position of Social Sharing widget'),
       '#weight' => 50,
       '#default_value' => variable_get('socialsharing_vertical_position', 0),
       '#options' => array(
          0 => t('Top Left'),
          1 => t('Top Right'),
          2 => t('Bottom Left'),
          3 => t('Bottom Right'),
        ),
    );
    $form[$group]['socialsharing_vertical_images']['socialsharing_vertical_position']['socialsharing_vertical_topweight'] = array(
      '#type' => 'textfield',
      '#title' => t('Specify distance of vertical sharing interface from top (Leave empty for default behaviour)'),
      '#weight' => 100,
      '#size' => 0,
      '#default_value' => variable_get('socialsharing_vertical_topweight'),
      '#description' => t('Enter a number (For example - 200). It will set the top CSS attribute of the interface to the value specified. Increase in the number pushes interface towards bottom.'),
    );
    $form[$group]['socialsharing_horizontal_location'] = array(
      '#title' => t('Location'),
      '#type' => 'radios',
      '#options' => array(
        'content' => t('Node content'),
        'block' => t('Block'),
        'links' => t('Links area'),
      ),
      '#default_value' => variable_get('socialsharing_horizontal_location', 'content'),
      '#description' => t('Select where the Social Share widget should appear. When selected to display as a block, you must choose which region to display the <em>Social Sharing block</em> in from the <a target="_blank" href="@blockssettings">Blocks Settings</a>.', array('@blockssettings' => url('admin/structure/block'))),
      '#attributes' => array('onclick' => 'display_block_horizontal(this.value);'),
    );
    $form[$group]['socialsharing_vertical_location'] = array(
      '#title' => t('Location'),
      '#type' => 'radios',
      '#options' => array(
        'content' => t('Node content'),
        'block' => t('Block'),
      ),
      '#default_value' => variable_get('socialsharing_vertical_location', 'content'),
      '#description' => t('Select where the Social Share widget should appear. When selected to display as a block, you must choose which region to display the <em>Social Share block</em> in from the <a target="_blank" href="@blockssettings">Blocks Settings</a>.', array('@blockssettings' => url('admin/structure/block'))),
      '#attributes' => array('onclick' => 'display_block_vertical(this.value);'),
    );
    $form[$group]['socialsharing_position_top'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show at the top of content.'),
      '#default_value' => variable_get('socialsharing_position_top', 1) ? 1 : 0,
      '#prefix' => '<div id="horizontal_sharing_show"> <b>Select the position of Social sharing interface</b>',
    );
    $form[$group]['socialsharing_position_bottom'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show at the bottom of content.'),
      '#default_value' => variable_get('socialsharing_position_bottom', 1) ? 1 : 0,
      '#suffix' => '</div>',
    );
    $form[$group]['socialsharing_top_weight'] = array(
      '#title' => t('Select the Weight of Social Sharing widget at the top of content'),
      '#description' => t('The weight of the widget determines the location on the page where it will appear.'),
      '#required' => FALSE,
      '#type' => 'select',
      '#options' => drupal_map_assoc(array(-100, -50, -25, -10, 0)),
      '#default_value' => variable_get('socialsharing_top_weight', -10),
    );
    $form[$group]['socialsharing_bottom_weight'] = array(
      '#title' => t('Select the Weight of Social Sharing widget at the bottom of content'),
      '#description' => t('The weight of the widget determines the location on the page where it will appear.'),
      '#required' => FALSE,
      '#type' => 'select',
      '#options' => drupal_map_assoc(array(0, 10, 25, 50, 100)),
      '#default_value' => variable_get('socialsharing_bottom_weight', 10),
    );
    $form[$group]['socialsharing_show_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Show Social Share on specific pages'),
      '#default_value' => variable_get('socialsharing_show_pages', 0),
      '#options' => array(
        0 => t('All pages except those listed'),
        1 => t('Only the listed pages'),
       ),
    );
    $form[$group]['socialsharing_show_exceptpages'] = array(
      '#type' => 'textarea',
      '#default_value' => variable_get('socialsharing_show_exceptpages', ''),
      '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
      '#rows' => 5,
    );
    $form[$group]['socialsharing_vertical_show_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Show Social Share on specific pages'),
      '#default_value' => variable_get('socialsharing_vertical_show_pages', 0),
      '#options' => array(
         0 => t('All pages except those listed'),
         1 => t('Only the listed pages'),
       ),
    );
    $form[$group]['socialsharing_vertical_show_exceptpages'] = array(
      '#type' => 'textarea',
      '#default_value' => variable_get('socialsharing_vertical_show_exceptpages', ''),
      '#description' => t('Enter a page title(you give on page creation) or node id (if url is http://example.com/node/1 then enter 1(node id)) with comma sepreted'),
      '#rows' => 5,
      '#suffix' => '</div>',
    );
    $form['#submit'][] = 'socialsharingbar_settings_submit';
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  return $form;
}
 /**
 * Submit the Social share settings.
 */
function socialsharingbar_settings_submit($form, &$form_state) {
   if (isset($form_state['input']['socialsharing_rearrange_providers_list'])) {
     variable_set('socialsharing_rearrange_providers_list', array_filter($form_state['input']['socialsharing_rearrange_providers_list']));
   } 
   else {
     variable_set('socialsharing_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", "2" => "google", "3" => "pinterest", "4" => "twitter", ));
   }
   if (!empty($form_state['input']['socialsharingcounter_rearrange_providers_list'])) {
     variable_set('socialsharingcounter_rearrange_providers_list', array_filter($form_state['input']['socialsharingcounter_rearrange_providers_list']));
   } 
   else {
     variable_set('socialsharingcounter_rearrange_providers_list', array( "0" => "Facebook Like", "1" => "Google+ +1", "2" => "Pinterest Pin it" , "3" => "LinkedIn Share", ));
   }
    if (isset($form_state['input']['socialsharing_vertical_rearrange_providers_list'])) {
     variable_set('socialsharing_vertical_rearrange_providers_list', array_filter($form_state['input']['socialsharing_vertical_rearrange_providers_list']));
   } 
   else {
     variable_set('socialsharing_vertical_rearrange_providers_list', array( "0" => "facebook", "1" => "googleplus", "2" => "google", "3" => "pinterest", "4" => "twitter", ));
   }
    if (!empty($form_state['input']['socialsharingcounter_vertical_rearrange_providers_list'])) {
     variable_set('socialsharingcounter_vertical_rearrange_providers_list', array_filter($form_state['input']['socialsharingcounter_vertical_rearrange_providers_list']));
   } 
   else {
     variable_set('socialsharingcounter_vertical_rearrange_providers_list', array( "0" => "Facebook Like", "1" => "Google+ +1", "2" => "Pinterest Pin it", "3" => "LinkedIn Share", ));
   }
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  $api_key = trim(variable_get('socialsharing_api_key'));
  if (!empty($api_key) && !preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $api_key)) {
    drupal_set_message(t('Your LoginRadius API Key is not valid, please correct it or contact LoginRadius support at <a href="@get_socialloginandsocialshare" target="_blank">www.Loginradius.com</a>', array('@get_socialloginandsocialshare' => 'http://www.Loginradius.com/')), 'error');
  }
  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
}