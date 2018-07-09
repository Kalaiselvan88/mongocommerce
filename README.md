"Mongo Commerce" 
The objective of the module is to extend the support of Commerce Kickstart to handle product catalog from MongoDB mainly to cater the rising need for Big Data support and provide an in house facility within Commerce Kickstart which can interact and manipulate the products in a flat database like MongoDB.Only the product data and related field which belong only to the product entity type shall be moved to MongoDB.
The solution developed here shall enhance and extend the Drupal Commerce layer to support MongoDB. Here the base commerce product module shall be altered to provide more capability to operate and handle MongoDB directly without the need for any third party module. 

Below entry is required in your settings.php in order for Drupal to connect to MongoDB.
#MongoDB
$conf['mongodb_connections'] = array(
  // Connection name/alias
  'default' => array(
    // Database name
    'db' => 'drupal_mongo',
  ),
);
//Default Logs
$conf['mongodb_watchdog'] = 'drupalogs';

NOTE: This module has been written to work without the demo store of Commerce Kickstart since the backend would
be MongoDB