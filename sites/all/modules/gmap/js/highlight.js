/**
 * @file
 * Common marker highlighting routines.
 */

/**
 * Highlights marker on rollover.
 * Removes highlight on previous marker.
 *
 * Creates a "circle" at the given point
 * Circle is global variable as there is only one highlighted marker at a time
 * and we want to remove the previously placed polygon before placing a new one.
 *
 * Original code "Google Maps JavaScript API Example"
 * JN201304:
 *    converted rpolygons to circles (not using the shapes.js API, should we be?)
 *    move marker highlight events to custom handler here, to handle radius in pixels (note behavior.radiusInMeters to skip geodesic calcs)
 *    removed google.events and moved events to gmaps binds
 *    added overlay object for creating a shape based on pixels instead of meters (seems to be the use case?)
 *    added gmaps binds for marker higlights, and general highlights.
 *
 * You can add highlights to a map with:
 *    obj.change('addHighlight',-1, {latitude:#, longitude:#} );
 * You can highlight a marker with:
 *    obj.change('markerHighlight',-1, marker);
 *      marker: that marker object used when creating the marker.  It can have options set at marker.highlight
 *
 * A Highlight object has to have either a <LatLng>Position or a <Number>latitude and <Number>longitude
 * Note the new highlight options = {
 *       radius: 10, // radius in pixels
 *       color: '#777777',
 *       weight: 2,
 *       opacity: 0.7,
 *       behavior: {
 *          draggable: false,
 *          editable: false,
 *       }
 *       opts: { actual google.maps.Circle opts can be put here for super custom cases }
 * }
 */

Drupal.gmap.factory.highlight = function (options) {
    return new google.maps.Circle(options);
}

Drupal.gmap.addHandler('gmap', function (elem) {
    var obj = this;
    obj.highlights = {};

    var overlayHandler = function (map, highlight) {
        this.setMap(map);
        this.highlight = highlight;
    }
    overlayHandler.prototype = new google.maps.OverlayView();
    overlayHandler.prototype.onAdd = function () {
        var highlight = this.highlight;
        if (!highlight.opts) {
            highlight.opts = {};
        } // sanity
        if (!highlight.behavior) {
            highlight.behavior = {};
        } // sanity
        if (!highlight.position) {
            highlight.position = new google.maps.LatLng(highlight.latitude, highlight.longitude);
        } // if you have a pos already then use it, otherwise gimme a lat/lon

        $.each({ // collect the options from either the highligh.opts object, from the passed target value, as a behavior or a default value
            radius: {target: 'radius', default: 10}, // radius in pixels
            strokeColor: {target: 'border', default: '#777777'},
            strokeWeight: {target: 'weight', default: 2},
            strokeOpacity: {target: 'opacity', default: 0.7},
            fillColor: {target: 'color', default: '#777777'},
            fillOpacity: {target: 'opacity', default: 0.7},
            draggable: {behavior: 'draggable', default: false},
            editable: {behavior: 'editable', default: false},
        }, function (key, config) {
            if (highlight.opts[key]) { // options was passed in
                return true;
            }
            else if (config.target && highlight[ config.target ]) { // highight.target should have a value
                highlight.opts[key] = highlight[ config.target ];
            }
            else if (config.behavior && highlight.behavior && highlight.behavior[ config.behavior ]) { // value is a behaviour
                highlight.opts[key] = highlight.behavior[ config.behavior ];
            }
            else if (config.default) { // default valuee
                highlight.opts[key] = config.default;
            }
        });

        highlight.opts.map = this.map;
        highlight.opts.center = highlight.position;

    }
    overlayHandler.prototype.draw = function () {
        var highlight = this.highlight;
        if (!this.highlight.behavior.radiusInMeters) {
            var projection = this.getProjection();
            var mapZoom = this.map.getZoom();
            var center = projection.fromLatLngToDivPixel(highlight.opts.center, mapZoom);
            var radius = highlight.opts.radius;
            var radial = projection.fromDivPixelToLatLng(new google.maps.Point(center.x, center.y + radius), mapZoom); // find a point that is the radius distance away in pixels
            highlight.opts.radius = google.maps.geometry.spherical.computeDistanceBetween(highlight.opts.center, radial);
            highlight.behavior.radiusInMeters = true;
        }
        highlight.highlight = Drupal.gmap.factory.highlight(highlight.opts);
    }
    overlayHandler.prototype.onRemove = function () {
        if (this.highlight.highlight) {
            this.highlight.highlight.setMap(null);
        }
    }

    Drupal.gmap.highlight = function (highlight) {
        new overlayHandler(obj.map, highlight); // all actions happen in an overlayview, so that we can input pixels for radius instead of meters and positions
    };
    Drupal.gmap.unhighlight = function (highlight) { // this can take an object that has a highlight, or a highlight/circle object
        if (highlight.highlight && highlight.highlight.setMap) {
            highlight.highlight.setMap(null);
        }
        else if (highlight.setMap) {
            highlight.setMap(null);
        }
    }

    // set and remove map highlights

    obj.bind('addHighlight', function (highlight) {
        Drupal.gmap.highlight(highlight);
    });
    obj.bind('removeHighlight', function (highlight) {
        Drupal.gmap.unhighlight(highlight);
    });

    // Marker specific code:
    var activeMarker; // remember that last marker activated.  In the default case we only allow one highlighted marker at a time
    obj.bind('markerHighlight', function (marker) {
        if (activeMarker && !obj.vars.behavior.allowMultipleMarkerHighlight) {
            obj.change('markerUnHighlight', -1, activeMarker);
        } // deactivate the active marker

        // If the highlight arg option is used in views highlight the marker.
        if (!marker.highlight) {
            marker.highlight = {}
        }
        if (!marker.highlight.color && obj.vars.styles.highlight_color) {
            marker.highlight.color = '#' + obj.vars.styles.highlight_color;
        }
        marker.highlight.position = marker.marker.getPosition();
        Drupal.gmap.highlight(marker.highlight);
        activeMarker = marker;
    });
    obj.bind('markerUnHighlight', function (marker) {
        if (!marker) {
            marker = activeMarker;
        } // remove the active marker if no marker is passed in
        if (activeMarker === marker) {
            activeMarker = null;
        }
        if (marker.highlight) {
            Drupal.gmap.unhighlight(marker.highlight);
        }
    });

    /**
     * Marker Binds
     *
     * Marker highlight code has been moved to this file from the marker.js
     *
     * Note that we rely on the obj.vars.behavior.highlight var to
     * decide if should highlight markers on events.
     * This decision could be made as an outer if conditional, instead
     * of repeated inside each bind, but this arrangement allows for
     * the behaviour to change, at a small cost.
     */

    obj.bind('addmarker', function (marker) {
        if (obj.vars.behavior.highlight) {
            google.maps.event.addListener(marker.marker, 'mouseover', function () {
                obj.change('markerHighlight', -1, marker);
            });
            google.maps.event.addListener(marker.marker, 'mouseout', function () {
                obj.change('markerUnHighlight', -1, marker);
            });
        }
        // If the highlight arg option is used in views highlight the marker.
        if (marker.opts.highlight == 1) {
            obj.change('markerHighlight', -1, marker);
        }
    });

// Originally I moved mouse highlights to the extra event binds before I realized that there is likely a usecase for highlights without enabling extra events
//   obj.bind('mouseovermarker', function(marker) {
//     if (obj.vars.behavior.highlight && marker) {
//       obj.change('markerHighlight',-1,marker);
//     }
//   });
//   obj.bind('mouseoutmarker', function(marker) {
//     if (obj.vars.behavior.highlight && marker) {
//       obj.change('markerUnHighlight',-1,marker);
//     }
//   });

});
