api = 2
core = 7.x

;--------------------
; Build Core
;--------------------

projects[drupal][version] = 7.21
projects[drupal][patch][] = "local:///patches/drupal/locale_import_overwrite.patch"
projects[drupal][patch][] = "local:///patches/drupal/no_uri_warning.patch"
projects[drupal][patch][] = "local:///patches/drupal/user_list_views_override_break_user_create.patch"
;http://drupal.org/node/1232416
projects[drupal][patch][] = "local:///patches/drupal/core-js-drupal-log-1232416-100-D7.patch"

projects[solution_core][type] = "profile"
projects[solution_core][download][type] = "git"
projects[solution_core][download][url] = "git://github.com/goruha/DrupalSolutionCore7.git"
projects[solution_core][download][tag] = 2.1
projects[solution_core][l10n_path] = "http://ftp.drupal.org/files/translations/7.x/drupal/drupal-7.21.%language.po"

projects[admin_menu][subdir] = "contrib"
projects[admin_menu][version] = "3.0-rc4"

projects[environments_custom][type] = "module"
projects[environments_custom][subdir]=contrib
projects[environments_custom][download][type] = "git"
projects[environments_custom][download][url] = "git://github.com/goruha/environments_custom.git"
projects[environments_custom][download][branch] = 7.x-1.x

projects[ctools][subdir]=contrib
projects[ctools][version]="1.3"

projects[module_filter][subdir]=contrib
projects[module_filter][version]="1.7"


projects[features][version] = "2.0-rc1"
projects[features][subdir] = "contrib"
; http://drupal.org/node/1599188
; projects[features][patch][] = "patches/features/features_rc3_empty_drupal_codestyle.patch"
; http://drupal.org/node/1666540
projects[features][patch][] = http://drupal.org/files/features_rc3_field_alter_hooks.patch
;projects[features][patch][] = "patches/features/features_permission_non_exited_module.patch"

projects[uuid][subdir]=contrib
projects[uuid][version]="1.0-alpha4"

projects[devel][subdir] = "contrib"
projects[devel][version] = "1.3"

projects[entity][subdir]=contrib
projects[entity][version]="1.1"

projects[entityreference][subdir]=contrib
projects[entityreference][version]="1.0"

projects[strongarm][subdir]=contrib
projects[strongarm][version]="2.0"
