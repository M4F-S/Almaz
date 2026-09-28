#ifndef TIMELINE_ANOMALY_H
#define TIMELINE_ANOMALY_H

#include "belya_agent.h"

/* Week-5 autonomy: anomaly detection over agent_timeline.
 * Rolling per-event_type daily counts over the last 7 days; today's count
 * is anomalous when it exceeds mean + 3*sigma (sigma > 0, count >= 5).
 * On anomaly: timeline entry + ANOMALY_REMEDIATION goal (deduped).
 * Returns number of anomalies found (>=0). */

int timeline_anomaly_scan(BelyaAgent *agent);

#endif /* TIMELINE_ANOMALY_H */