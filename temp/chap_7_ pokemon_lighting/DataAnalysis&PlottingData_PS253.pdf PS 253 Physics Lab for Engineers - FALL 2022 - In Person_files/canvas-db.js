////////////////////////////////////////////////////
// DESIGN TOOLS CONFIG                            //
////////////////////////////////////////////////////
// Copyright (C) 2016  Utah State University
var DT_variables = {
    iframeID: '',
    // Path to the hosted USU Design Tools
    path: 'https://designtools.ciditools.com/',
    templateCourse: '71217',
    // OPTIONAL: Button will be hidden from view until launched using shortcut keys
    hideButton: true,
    // OPTIONAL: Limit tools loading by users role
    limitByRole: false, // set to true to limit to roles in the roleArray
    // adjust roles as needed
    roleArray: [
        'student',
        'teacher',
        'admin'
    ],
    // OPTIONAL: Limit tools to an array of Canvas user IDs
    limitByUser: false, // Change to true to limit by user
    // add users to array (Canvas user ID not SIS user ID)
    userArray: [
        '1234',
        '987654'
    
    ]
};

// Run the necessary code when a page loads
$(document).ready(function () {
'use strict';
// This runs code that looks at each page and determines what controls to create
$.getScript(DT_variables.path + 'js/master_controls.js', function () {
    console.log('master_controls.js loaded');
});
});
////////////////////////////////////////////////////
// END DESIGN TOOLS CONFIG                        //
////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Make Multi Tool Visible to Admins           //
/////////////////////////////////////////////////
$(document).ready(function () {
'use strict';
var multiToolExternalID = '42712', // external tool id
    // What roles can see the Multi Tool
    multiToolUserRoles = [
        'admin'
    ];
// Also hidden using CSS, this is to account for currently cached files
$('.context_external_tool_' + multiToolExternalID).hide();
$('#nav_edit_tab_id_context_external_tool_' + multiToolExternalID).hide();
$('#section-tabs a:contains(Multi Tool)').hide();
// If current user has appropriate role, show the Multi Tool
$.each(multiToolUserRoles, function (ignore, val) {
    if ($.inArray(val, ENV.current_user_roles) > -1) {
        $('.context_external_tool_' + multiToolExternalID).show();
        $('#nav_edit_tab_id_context_external_tool_' + multiToolExternalID).show();
    }
});

// Add Hunt Library link / modal to Course navigation
var hunt_library_modal = document.createElement( 'div' );
$( hunt_library_modal ).html( '\
    <p>The Hunt Library is your starting point for&nbsp;research. Find articles, books, videos and more using the library&nbsp;website. For&nbsp;help, please contact <a target="_blank" href="http://guides.erau.edu/ask-a-librarian">Ask&nbsp;a&nbsp;Librarian</a>.</p>\
    <p><a class="btn btn-small btn-primary" target="_blank" href="http://huntlibrary.erau.edu/">Visit Hunt Library Website</a></p>\
    <ul>\
        <li><a target="_blank" href="http://guides.erau.edu/az.php">Research Databases</a></li>\
        <li><a target="_blank" href="http://guides.erau.edu/">Research Guides</a></li>\
        <li><a target="_blank" href="https://huntlibrary.erau.edu/help/library-tutorials">Library Tutorials</a></li>\
    </ul>\
' );
var hunt_library_button = document.createElement( 'li' );
$( hunt_library_button ).html( '<a href="#" id="hunt-lib-button">Hunt Library</a>' );
$( hunt_library_button ).find( 'a' ).click(function() {
    $( this ).blur();
    $( hunt_library_modal ).dialog({
        title: "Hunt Library",
        width: 400,
        resizable: !1
    });
    event.preventDefault();
    event.stopPropagation();
});

var section_tabs = $( '#section-tabs' );
var last_section_tab = $( section_tabs ).children().last();

if ( last_section_tab.text() == 'Settings' ) {
    last_section_tab.before( hunt_library_button );
} else {
    section_tabs.append( hunt_library_button );
}
});
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Rename Syllabus to Summary                  //
/////////////////////////////////////////////////
$(document).ready(function () {
	linkText = $('#section-tabs a[href*="/assignments/syllabus"].syllabus').html();
	if (linkText != null && linkText != undefined) {
		linkText = linkText.replace("Syllabus", "Summary");
	}
	$('#section-tabs a[href*="/assignments/syllabus"].syllabus').html(linkText).attr("title", "Summary");
	// Same change in Navigation > Settings
	if ($('#nav_edit_tab_id_1').length) {
		$('#nav_edit_tab_id_1').html($('#nav_edit_tab_id_1').html().replace(/Syllabus/g, "Summary")).attr("aria-label", "Summary");
	}
	if (window.location.pathname.search("/assignments/syllabus") > 0) {
		var title = window.document.title;
		title = title.replace(/Syllabus/g, "Summary");
		window.document.title = title;
	}
});
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Rename Simple Syllabus to Syllabus          //
/////////////////////////////////////////////////
$(document).ready(function () {
	$('#section-tabs a[href*="/external_tools/74776"]').text("Syllabus").attr("title", "Syllabus");
	// Same change in Navigation > Settings
	if ($('#nav_edit_tab_id_context_external_tool_74776').length) {
		$('#nav_edit_tab_id_context_external_tool_74776').html($('#nav_edit_tab_id_context_external_tool_74776').html().replace(/Simple Syllabus/g, "Syllabus")).attr("aria-label", "Syllabus");
	}
	// Also rename the window Title
	if (window.location.pathname.search("external_tools/74776") > 0) {
		var title = window.document.title;
		title = title.replace(/Simple Syllabus/g, "Syllabus");
		window.document.title = title;
	}
});
/////////////////////////////////////////////////
/////////////////////////////////////////////////